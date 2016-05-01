#ifndef TAKENGINE_H
#define TAKENGINE_H

#include <string>
#include <vector>
#include "board.h"
#include "move.h"

using std::string;
using std::vector;

class TakEngine
{
    public:
        Board currentBoard;
        int boardSize;
        int maxPieces;
        int maxCapstones;
        vector<Move> moves;

        TakEngine(int size, bool white);
        bool tryMove(Move move);
        int gameDone();
        void printBoard();
        bool whiteTurn();
};

#endif
