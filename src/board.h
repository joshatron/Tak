#ifndef BOARD_H
#define BOARD_H

#include <string>
#include "move.h"

using std::string;

class Board
{
    public:
        string board[10][10];
        int boardSize;
        int whitePieces;
        int whiteCapstones;
        int blackPieces;
        int blackCapstones;
        bool whiteMove;
        int firstMove;

        Board(int size, int maxPieces, int maxCapstones, bool white);
        Board();
        bool updateBoard(Move move);
        int gameDone();
        void print();
};

#endif
