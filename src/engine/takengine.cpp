#include "takengine.h"

TakEngine::TakEngine(int size, bool white)
{
    boardSize = size;

    switch(size)
    {
        case 3:
            maxPieces = 10;
            maxCapstones = 0;
            break;
        case 4:
            maxPieces = 15;
            maxCapstones = 0;
            break;
        case 5:
            maxPieces = 21;
            maxCapstones = 1;
            break;
        case 6:
            maxPieces = 30;
            maxCapstones = 1;
            break;
        case 7:
            maxPieces = 40;
            maxCapstones = 1;
            break;
        case 8:
            maxPieces = 50;
            maxCapstones = 2;
            break;
    }

    currentBoard = Board(size, maxPieces, maxCapstones, white);
}

TakEngine::TakEngine()
{
    boardSize = 5;
    maxPieces = 21;
    maxCapstones = 1;
    currentBoard = Board(boardSize, maxPieces, maxCapstones, true);
}

bool TakEngine::tryMove(Move move)
{
    bool result = currentBoard.updateBoard(move);
    if(result)
    {
        moves.push_back(move);
    }

    return result;
}

int TakEngine::gameDone()
{
    return currentBoard.gameDone();
}

void TakEngine::printBoard()
{
    currentBoard.print();
}

bool TakEngine::whiteTurn()
{
    return currentBoard.whiteMove;
}

int TakEngine::numberAtSpot(int x, int y)
{
    return currentBoard.board[x+1][y+1].length();
}
