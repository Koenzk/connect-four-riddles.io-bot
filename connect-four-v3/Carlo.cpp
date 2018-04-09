//
// Created by koenzk on 27-3-18.
//

#include "Carlo.h"

//  State:
//  0 .......
//  1 .......
//  2 .......
//  3 .......
//  4 .......
//  5 .......
//    0123456

Player Carlo::getDisc(State &s, int col, int row) {
    return s[row][col];
}

void Carlo::setDisc(State &s, int col, int row, Player player) {
    s[row][col] = player;
}

bool Carlo::colIsFull(State &s, int col) {
    return getDisc(s, col, 0) != Player::None;
}

bool Carlo::dropDisc(State &s, int col, Player player) {
    if (colIsFull(s, col)) return false;
    for (int row = 5; row >= 0; row--) {
        if (getDisc(s, col, row) == Player::None) {
            setDisc(s, col, row, player);
            break;
        }
    }
    return true;
}

Player Carlo::randomGame(State &s, Player onTurn) {
    for (int i=0; i < 500; i++) { // TODO: Can much more efficient
        if (dropDisc(s, rand()%7, onTurn)) {
            onTurn = (onTurn == Player::O) ? Player::X : Player::O;

            Player winner = getWinner(s);
            if (winner != Player::None) return winner;
        }
    }
    return Player::None;
}

int Carlo::suggestMove(const State &s, Player onTurn) {
    State result = s;

    int best = -1;
    double best_ratio = 0;
    int games_per_move = 20000; // TODO: Find value as high as possible

    for (int move = 0; move < 7; move++) {
        if (colIsFull(result, move)) continue; // No valid move.

        int won = 0, lost = 0;
        for (int j = 0; j < games_per_move; j++) {
            State copy = result;
            dropDisc(copy, move, onTurn);

            if (getWinner(copy) == onTurn) return move; // return if this is a winning move

            Player next = (onTurn == Player::O) ? Player::X : Player::O;
            Player winner = randomGame(copy, next);

            if (winner == Player::O || winner == Player::X) {
                if (winner == onTurn) {
                    won++;
                } else {
                    lost++;
                }
            }
        }

        double ratio = (double)won/(lost+1);
        if (ratio > best_ratio || best == -1) {
            best = move;
            best_ratio = ratio;
        }
    }
    return best;
}
