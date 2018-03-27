// c4bot.cpp
// Aswin van Woudenberg

#include "c4bot.h"
#include "node.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <limits>

void C4Bot::run() {
    srand(time(NULL));
    std::string line;
    while (std::getline(std::cin, line)) {
        std::vector<std::string> command = split(line, ' ');
        if (command[0] == "settings") {
            setting(command[1], command[2]);
        } else if (command[0] == "update" && command[1] == "game") {
            update(command[2], command[3]);
        } else if (command[0] == "action" && command[1] == "move") {
            move(std::stoi(command[2]));
        } else {
            std::cerr << "Unknown command: " << line << std::endl;
        }
    }
}

int C4Bot::getTimeElapsed() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
}

double C4Bot::selectfn(Node* n) {
    double vi = n->getUtility();
    int ni = n->getVisits();
    int np = n->getParent()->getVisits();
    return (vi + sqrt(2 * log(np) / ni));
}

double C4Bot::selectfnOP(Node* n) { //modified UCT so that the more you visit a node, the greater the score
    double vi = n->getUtility();
    int ni = n->getVisits();
    int np = n->getParent()->getVisits();
    return (vi - sqrt(2 * log(np) / ni));
}

void C4Bot::backPropagate(Node* n, int score) {
    n->visit();
    n->addUtility(score);
    if (n->getParent()) {
        backPropagate(n->getParent(), score);
    }
}

int C4Bot::simulate(State s){
    Player terminal = getWinner(s);
    if((terminal == Player::O && your_botid == 0) || (terminal == Player::X && your_botid == 1)){//lose
        return 0;
    } else if ((terminal == Player::X && your_botid == 0) || (terminal == Player::O && your_botid == 1)){ //win
        return 100;
    }
    std::vector<Move> moves = getMoves(s);
    if (moves.size() == 0){ //draw
        return 50;
    }
    Move best = moves.at(std::rand()%(moves.size()));
    s = doMove(s, best);
    return simulate(s);
}

void C4Bot::expand(Node* n) {
    std::vector<Move> moves = getMoves(state);
    for (size_t i = 0; i < moves.size(); i++) { //simulates player move
        State s = doMove(state, moves.at(i));
        Node* newNode = new Node(n, s, moves.at(i), 0, 0, 2);
        n->addChild(newNode);
        if (getWinner(s) == Player::None && getMoves(n->getState()).size() > 0) {
            std::vector<Move> movesOP = getMoves(s);
            for (size_t j = 0; j < movesOP.size(); j++) {
                State newState = doMove(s, movesOP.at(j));
                Node* newNodeOP = new Node(newNode, newState, movesOP.at(j), 0, 0, 1);
                newNode->addChild(newNodeOP);
            }
        }
    }
}

Node* C4Bot::select(Node* n) {
    if (n->getVisits() == 0 || getWinner(n->getState()) != Player::None || getMoves(n->getState()).size() == 0) { //no visits or is terminal
        return n;
    }
    Children* minList = n->getChildren();
    for (size_t i = 0; i < minList->size(); i++) { //if a min node hasn't been visited, visit a random child of its
        if (minList->at(i)->getVisits() == 0) {
            return (minList->at(i));
        }
    }
    double score = 0;
    Node* result = minList->at(0);
    for (size_t i = 0; i < minList->size(); i++) { //chooses the min node with the highest score
        double newScore = selectfn(minList->at(i));
        if (newScore > score && result->getChildren()->size() > 0) {
            score = newScore;
            result = minList->at(i);
        }
    }
    Children* maxList = result->getChildren();
    for (size_t i = 0; i < maxList->size(); i++) {
        if (maxList->at(i)->getVisits() == 0) {//if a max node hasn't been visited, select that one
            return maxList->at(i);
        }
    }
    double score2 = std::numeric_limits<double>::max();
    Node* finalResult = nullptr;
    for (size_t i = 0; i < maxList->size(); i++) { //all max nodes have been visited, select the one with the lowest score
        double newScore = selectfnOP(maxList->at(i));
        if (newScore < score2) {
            score2 = newScore;
            finalResult = maxList->at(i);
        }
    }
    return finalResult;
}

Move C4Bot::makeMove(int timeout) {
    // std::vector<Move> moves = getMoves(state);
    // return *select_randomly(moves.begin(), moves.end());

    std::vector<Move> moves = getMoves(state);
    if (moves.size() == 1) {
        return moves.at(0);
    }
    Node initial{ nullptr, state, 0, 0, 0, 1 };
    while (timeout - getTimeElapsed() > 175) {
        Node* current = select(&initial);
        expand(current);
        int score = simulate(current->getState());
        backPropagate(current, score);
    }
    double bestScore = 0;
    Move move(0);
    for(size_t i = 0; i<initial.getChildren()->size(); i++){
        if (initial.getChildren()->at(i)->getUtility() > bestScore){
            bestScore = initial.getChildren()->at(i)->getUtility();
            move = initial.getChildren()->at(i)->getMove();
        }
    }
    return move;
}


void C4Bot::move(int timeout) {
    begin = std::chrono::steady_clock::now();
    Move m = makeMove(time_per_move);
    std::cout << "place_disc " << m << std::endl;
}

void C4Bot::update(std::string &key, std::string &value) {
    if (key == "round") {
        round = std::stoi(value);
    } else if (key == "field") {
        int row = 0;
        int col = 0;
        std::vector<std::string> fields = split(value, ',');
        for (std::string &field : fields) {
            if (field == "0") {
                state[row][col] = Player::X;
            } else if (field == "1") {
                state[row][col] = Player::O;
            } else {
                state[row][col] = Player::None;
            }
            col++;
            if (col == 7) {
                row++;
                col = 0;
            }
        }
    }
}

void C4Bot::setting(std::string &key, std::string &value) {
    if (key == "timebank") {
        timebank = std::stoi(value);
    } else if (key == "time_per_move") {
        time_per_move = std::stoi(value);
    } else if (key == "player_names") {
        std::vector<std::string> names = split(value, ',');
        player_names[0] = names[0];
        player_names[1] = names[1];
    } else if (key == "your_bot") {
        your_bot = value;
    } else if (key == "your_botid") {
        your_botid = std::stoi(value);
    } else if (key == "field_columns") {
        field_columns = std::stoi(value);
    } else if (key == "field_rows") {
        field_rows = std::stoi(value);
    }
}

std::vector<std::string> C4Bot::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

