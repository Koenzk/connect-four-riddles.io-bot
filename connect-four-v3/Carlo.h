//
// Created by koenzk on 27-3-18.
//

#ifndef CONNECT_FOUR_V3_CARLO_H
#define CONNECT_FOUR_V3_CARLO_H

#include "c4.h"

class Carlo {

    // Play a random game from a given State
    Player randomGame(State &s, Player onTurn);
    // Drop a disc in a specific row
    bool dropDisc(State &s, int col, Player player);
    // Get which Player (X, O or None) is at a specific position
    Player getDisc(State &s, int col, int row);
    // Set a player (X or O) at a specific position in a State
    void setDisc (State &s, int col, int row, Player player);
    // Check if a row is full
    bool colIsFull(State &s, int col);

public:

    // Suggest a move based on the Monte Carlo algorithm
    int suggestMove(const State &s, Player onTurn);

};


#endif //CONNECT_FOUR_V3_CARLO_H
