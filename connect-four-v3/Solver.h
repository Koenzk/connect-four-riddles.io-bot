//
// Created by koenzk on 28-3-18.
//

#ifndef CONNECT_FOUR_V3_SOLVER_H
#define CONNECT_FOUR_V3_SOLVER_H

#include "c4.h"

class Solver {

    State state = { { { { Player::None } } } };

    Player c4Get(State state, int col, int level);
    void c4Set(State state, int col, int row, Player player);

    void c4Copy(State *dst, State *src);
    bool c4ColIsFull(State state, int col);
    int c4Drop(State state, int col, Player player);
    Player c4GetWinner(State state);

    Player c4RandomGame(State state, Player tomove);

public:
    int c4SuggestMove(State state, Player tomove);
};


#endif //CONNECT_FOUR_V3_SOLVER_H
