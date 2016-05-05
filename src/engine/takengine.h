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
        //the current board
        Board currentBoard;
        //the dimension of the board (3-8)
        int boardSize;
        //the max number of pieces each player can have
        int maxPieces;
        //the max number of capstones each player can have
        int maxCapstones;
        //list of moves made, mainly for logging purposes
        vector<Move> moves;

        //constructor for new game
        //size- size of the board
        //white- true if white goes first
        TakEngine(int size, bool white);
        TakEngine();
        //try to make move
        //if move legal, returns true
        //if move illegal, returns false
        bool tryMove(Move move);
        //check if game done
        //return of 0- game not over
        //reutrn of 1- white won
        //return of 2- black won
        int gameDone();
        //print out the game board
        void printBoard();
        //true if it is currently white's turn
        bool whiteTurn();
        //returns how many pieces are on a spot
        int numberAtSpot(int x, int y);
        string stringAtSpot(int x, int y){return currentBoard.board[x+1][y+1];}
        int whitePiecesLeft(){return currentBoard.whitePieces;}
        int whiteCapstonesLeft(){return currentBoard.whiteCapstones;}
        int blackPiecesLeft(){return currentBoard.blackPieces;}
        int blackCapstonesLeft(){return currentBoard.blackCapstones;}
        bool firstTurns(){return (currentBoard.firstMove > 0) ? true : false;}
};

#endif
