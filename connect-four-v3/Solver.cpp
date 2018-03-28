//
// Created by koenzk on 28-3-18.
//

#include "Solver.h"

#include <cstring>

// Return what the specified col/level hole contains.
Player Solver::c4Get(State state, int col, int row) {
    return state[row][col];
}

// Set the specified col/level hole to the specified DISC value, see the DISC_* macros.
void Solver::c4Set(State state, int col, int row, Player player) {
    state[row][col] = player;
}

// Clean the board filling it with empty values.
//void Solver::c4Clean(State &state) {
//    memset(state,Player::None,sizeof(state));
//}

// Copy the source board to the destination board.
void Solver::c4Copy(State *dst, State *src) {
    memcpy(dst,src,sizeof(dst));
}

// Return true if the given colum is all full with DISCs.
bool Solver::c4ColIsFull(State state, int col) {
    return c4Get(state,col,5) != Player::None;
}

// Drop a DISC in the give column.
// If it's full 0 is returned, otherwise the first empty level is set to the specified value.
int Solver::c4Drop(State state, int col, Player player) {
    if (c4ColIsFull(state,col)) return 0;
    for (int row = 0; row < 6; row++) {
        if (c4Get(state,col,row) == Player::None) {
            c4Set(state,col,row,player);
            break;
        }
    }
    return 1;
}

// Return the winner color, or DISC_EMPTY if there is no winner.
// If the board is full but there is no winner, DISC_DRAW is returned.
Player Solver::c4GetWinner(State state) {
    int empty = 0;
    for (int row = 5; row >= 0; row--) {
        for (int col = 0; col < 7; col++) {
            Player player = c4Get(state,col,row);
            if (player == Player::None) {
                empty++;
                continue;
            }

            // There are four possible valid directions to make
            // circles in a row:
            struct {
                int col_incr;
                int level_incr;
            } dir[4] = {
                    {1,0},
                    {0,1},
                    {1,1},
                    {-1,1}
            };

            // Check if from this position there is a winning sequence.
            for (int d = 0; d < 4; d++) {
                // Seek the initial circle still having this color.
                int start_col = col;
                int start_level = row;
                while(c4Get(state,start_col-dir[d].col_incr,
                            start_level-dir[d].level_incr) == player)
                {
                    start_col -= dir[d].col_incr;
                    start_level -= dir[d].level_incr;
                }

                int count = 0; // Count consecutive circles.
                while(c4Get(state,start_col,start_level) == player) {
                    count++;
                    start_col += dir[d].col_incr;
                    start_level += dir[d].level_incr;
                }
                if (count >= 4) return player;
            }
        }
    }
    return empty ? Player::None : Player::Draw;
}

// Simulate a random game. The 'tomove' argument tells the function who
// is the player to move first. The board does not need to be empty, the random
// game is continued from the current state. The function returns the winner
// color, or DISC_DRAW if there is no winner.
Player Solver::c4RandomGame(State state, Player tomove) {
    while(true) {
        if (c4Drop(state,rand()%7,tomove)) {
            tomove = (tomove == Player::O) ? Player::X : Player::O;
        }
        Player winner = c4GetWinner(state);
        if (winner != Player::None) return winner;
    }
}

// Suggest a move among the (at max) 7 possible. For each move we play random games
// and store the ratio between won and lost games. Finally the move with the best
// ratio is returned. If there is no valid moves, -1 is returned.
int Solver::c4SuggestMove(State state, Player tomove) {
    int best = -1;

    double best_ratio = 0;
    int games_per_move = 10000; // TODO: Find perfect value
    for (int move = 0; move < 7; move++) {
        if (c4ColIsFull(state,move)) continue; /* No valid move. */
        int won = 0, lost = 0;
        for (int j = 0; j < games_per_move; j++) {
            State copy = { { { { Player::None } } } };
            c4Copy(&copy,&state);
            c4Drop(copy,move,tomove);
            // If this is a winning move, just return it.
            if (c4GetWinner(copy) == tomove) return move;
            Player next = (tomove == Player::O) ? Player::X : Player::O;
            Player winner = c4RandomGame(copy,next);
            if (winner == Player::O || winner == Player::X) {
                if (winner == tomove) {
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
