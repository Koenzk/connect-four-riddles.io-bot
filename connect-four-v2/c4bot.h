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

    struct board {
        char holes[6*7];
    } board;

    int c4Get(board *b, int col, int level);
    void c4Set(board *b, int col, int level, int value);
    void c4Clean(board *b);
    void c4Copy(board *dst, board *src);
    int c4ColIsFull(board *b, int col);
    int c4Drop(board *b, int col, int value);
    void c4Print(board *b);
    int c4GetWinner(board *b);
    int c4RandomGame(board *b, int tomove);
    int c4SuggestMove(board *b, int tomove);


    std::vector<std::string> fields;

    std::vector<std::string> split(const std::string &s, char delim);
    void setting(std::string &key, std::string &value);
    void update(std::string &key, std::string &value);
    void move(int timeout);

public:
    void run();
};

#endif // C4BOT_H
