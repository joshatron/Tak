#ifndef BOARD_H
#define BOARD_H

#include <string>
#include "move.h"

using std::string;

//representation of tak board
class Board
{
    public:
        //the board
        string board[10][10];
        //the board dimension(3-8)
        int boardSize;
        //how many normal pieces white has off board
        int whitePieces;
        //how many capstones white has off board
        int whiteCapstones;
        //how many normal pieces black has off board
        int blackPieces;
        //how many capstones black has off board
        int blackCapstones;
        //whether it is white's move
        bool whiteMove;
        //starts as 2 and goes down each turn till 0
        //while > 0, players place other team's piece
        int firstMove;

        //main constructor
        //size- board size
        //maxPiece- starting number of normal pieces per player
        //maxCapstones- starting number of capstones per player
        //white- true if white goes first
        Board(int size, int maxPieces, int maxCapstones, bool white);
        //default constructor, that makes a 5x5 game
        Board();
        //attempts to make move
        //if legal, makes move and returns true
        //if illegal, returns false
        bool updateBoard(Move move);
        //checks if someone won
        //return of 0 means game not done
        //return of 1 means white won
        //return or 2 means black won
        int gameDone();
        //print out game board
        void print();

    private:
        bool isValidMove(Move move);
        bool validPlaceMove(Move move);
        bool validMoveMove(Move move);

        int roadDone();
        int checkOneRoad(int x, int y, int finalX, int finalY);
        int roadBFS(int (*visited)[10], int x, int y, int finalX, int finalY, bool whiteRoad);
        int boardFilled();
        int noPieces();
        int greaterLand();
};

#endif
