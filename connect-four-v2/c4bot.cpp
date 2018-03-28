// c4bot.cpp
// Aswin van Woudenberg
// Koen Lukkien

#include "c4bot.h"
#include "MonteCarloSolver.h"

#include <iostream>
#include <sstream>

MonteCarloSolver solver;

//struct board {
//    char holes[6*7];
//} board;
//
void C4Bot::boardSet(board *b, int col, int row, int player) {
//    if (col < 0 || col > 6 || row < 0 || row > 5) return;
    b->holes[(5*7)+col-(row*7)] = player;
}

void C4Bot::run() {
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

void C4Bot::move(int timeout) {
    // Do something more intelligent here instead of returning a random move
    std::vector<Move> moves = getMoves(state);
    std::cout << "place_disc " << *select_randomly(moves.begin(), moves.end()) << std::endl;

    std::cout << "place_disc " << solver.makeMove(board, your_botid + 1) << std::endl;
}

void C4Bot::update(std::string &key, std::string &value) {
    if (key == "round") {
        round = std::stoi(value);
    } else if (key == "field") {
//        int row = 0;
//        int col = 0;
//        std::vector<std::string> fields =

        fields = split(value, ',');

//        for (std::string &field : fields) {
//            if (field == "0") {
//                state[row][col] = Player::X;
//            } else if (field == "1") {
//                state[row][col] = Player::O;
//            } else {
//                state[row][col] = Player::None;
//            }
//            col++;
//            if (col == 7) {
//                row++;
//                col = 0;
//            }
//        }
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
