// c4bot.h
// Aswin van Woudenberg

#ifndef C4BOT_H
#define C4BOT_H

#include <string>
#include <vector>
#include <chrono>

#include "c4.h"
#include "node.h"

class C4Bot {
    int timebank;
    int time_per_move;
    std::string player_names[2];
    std::string your_bot;
    int your_botid;
    int field_columns;
    int field_rows;

    int round;
    State state = { { { { Player::None } } } };

    typedef std::chrono::steady_clock::time_point Time;
    Time begin;

    std::vector<std::string> split(const std::string &s, char delim);
    void setting(std::string &key, std::string &value);
    void update(std::string &key, std::string &value);
    int getTimeElapsed();
    double selectfn(Node* n);
    double selectfnOP(Node* n);
    void backPropagate(Node* n, int score);
    int simulate(State s);
    void expand(Node* n);
    Node* select(Node* n);
    Move makeMove(int timeout);
    void move(int timeout);

public:
    void run();
};

#endif // C4BOT_H
