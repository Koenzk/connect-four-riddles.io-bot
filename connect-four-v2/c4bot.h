// c4bot.h
// Aswin van Woudenberg
// Koen Lukkien

#ifndef C4BOT_H
#define C4BOT_H

#include "c4.h"

#include <string>
#include <vector>

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

    std::vector<std::string> fields;

    struct board {
        char holes[6*7];
    } board;

    void boardSet(board *b, int col, int row, int player);

    std::vector<std::string> split(const std::string &s, char delim);
    void setting(std::string &key, std::string &value);
    void update(std::string &key, std::string &value);
    void move(int timeout);

public:
    void run();
};

#endif // C4BOT_H
