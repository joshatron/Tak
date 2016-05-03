#include "board.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

Board::Board(int size, int maxPieces, int maxCapstones, bool white)
{
    boardSize = size;
    whitePieces = maxPieces;
    whiteCapstones = maxCapstones;
    blackPieces = maxPieces;
    blackCapstones = maxCapstones;
    whiteMove = white;
    firstMove = 2;

    for(int k = 0; k < 10; k++)
    {
        for(int a = 0; a < 10; a++)
        {
            if(k == 0 || k == 9 || a == 0 || a == 9 || k > size || a > size)
            {
                board[k][a] = "-";
            }
            else
            {
                board[k][a] = "";
            }
        }
    }
}

Board::Board()
{
    boardSize = 5;
    whitePieces = 21;
    whiteCapstones = 1;
    blackPieces = 21;
    blackCapstones = 1;
    whiteMove = true;
    firstMove = 2;

    for(int k = 0; k < 10; k++)
    {
        for(int a = 0; a < 10; a++)
        {
            if(k == 0 || k == 9 || a == 0 || a == 9 || k > boardSize || a > boardSize)
            {
                board[k][a] = "-";
            }
            else
            {
                board[k][a] = "";
            }
        }
    }
}

bool Board::updateBoard(Move move)
{
    //if invalid, return false
    if(!isValidMove(move))
    {
        return false;
    }

    int x = move.x + 1;
    int y = move.y + 1;
    if(move.type.compare("place") == 0)
    {
        //white being placed
        if((whiteMove && firstMove == 0) || (!whiteMove && firstMove > 0))
        {
            if(move.placeType == 0)
            {
                board[x][y] = "f";
                whitePieces--;
            }
            else if(move.placeType == 1)
            {
                board[x][y] = "s";
                whitePieces--;
            }
            else
            {
                board[x][y] = "c";
                whiteCapstones--;
            }
        }
        //black being placed
        else
        {
            if(move.placeType == 0)
            {
                board[x][y] = "F";
                blackPieces--;
            }
            else if(move.placeType == 1)
            {
                board[x][y] = "S";
                blackPieces--;
            }
            else
            {
                board[x][y] = "C";
                blackCapstones--;
            }
        }
    }
    else
    {
        //set direction
        int dx = 0;
        int dy = 0;
        if(move.direction.compare("n") == 0)
        {
            dy = -1;
        }
        else if(move.direction.compare("s") == 0)
        {
            dy = 1;
        }
        else if(move.direction.compare("e") == 0)
        {
            dx = 1;
        }
        else
        {
            dx = -1;
        }

        int currX = x;
        int currY = y;
        string current = board[x][y];
        board[x][y] = "";
        for(int k = 0; k < move.distance; k++)
        {
            if(board[currX][currY].length() > 0)
            {
                //sit down standing stones
                char top = board[currX][currY].at(board[currX][currY].length() - 1);
                if(top == 's')
                {
                    board[currX][currY] = board[currX][currY].substr(0, board[currX][currY].length() - 1);
                    board[currX][currY] = board[currX][currY].append("f");
                }
                if(top == 'S')
                {
                    board[currX][currY] = board[currX][currY].substr(0, board[currX][currY].length() - 1);
                    board[currX][currY] = board[currX][currY].append("F");
                }
            }

            //place bottom of pile on top of spot
            board[currX][currY] = board[currX][currY].append(current.substr(0, move.toLeave[k]));
            current = current.substr(move.toLeave[k], current.length() - move.toLeave[k]);

            //move location
            currX += dx;
            currY += dy;
        }
    }

    //set up for next move
    if(firstMove > 0)
    {
        firstMove--;
    }
    whiteMove = !whiteMove;
    return true;
}

bool Board::isValidMove(Move move)
{
    int x = move.x + 1;
    int y = move.y + 1;

    //out of bounds
    if(x < 1 || x > boardSize || y < 1 || y > boardSize)
    {
        return false;
    }

    if(firstMove > 0)
    {
        //not placing at beginning
        if(move.type.compare("place") != 0)
        {
            return false;
        }

        //need to place a flat stone
        if(move.placeType != 0)
        {
            return false;
        }
    }

    //piece place
    if(move.type.compare("place") == 0)
    {
        return validPlaceMove(move);
    }
    //piece move
    else if(move.type.compare("move") == 0)
    {
        return validMoveMove(move);
    }
    //not valid move type
    else
    {
        return false;
    }
}

bool Board::validPlaceMove(Move move)
{
    int x = move.x + 1;
    int y = move.y + 1;

    //not valid piece type
    if(move.placeType < 0 || move.placeType > 2)
    {
        return false;
    }

    //not enough pieces
    if(whiteMove)
    {
        if((move.placeType < 2 && whitePieces == 0) || (move.placeType == 2 && whiteCapstones == 0))
        {
            return false;
        }
    }
    else
    {
        if((move.placeType < 2 && blackPieces == 0) || (move.placeType == 2 && blackCapstones == 0))
        {
            return false;
        }
    }

    //spot not empty
    if(board[x][y].length() > 0)
    {
        return false;
    }

    //valid place
    return true;
}

bool Board::validMoveMove(Move move)
{
    int x = move.x + 1;
    int y = move.y + 1;

    //spot empty
    if(board[x][y].length() == 0)
    {
        return false;
    }

    //top not owned by current player
    char top = board[x][y].at(board[x][y].length() - 1);
    if((top == 'f' || top == 's' || top == 'c') != whiteMove)
    {
        return false;
    }

    //not moving far enough
    if(move.distance <= 1)
    {
        return false;
    }

    //invalid direction
    if(move.direction.compare("n") != 0 && move.direction.compare("s") != 0 &&
       move.direction.compare("e") != 0 && move.direction.compare("w") != 0)
    {
        return false;
    }

    int dx = 0;
    int dy = 0;

    if(move.direction.compare("n") == 0)
    {
        dy = -1;
    }
    else if(move.direction.compare("s") == 0)
    {
        dy = 1;
    }
    else if(move.direction.compare("e") == 0)
    {
        dx = 1;
    }
    else if(move.direction.compare("w") == 0)
    {
        dx = -1;
    }

    int currX = x;
    int currY = y;
    string current = board[x][y];
    for(int k = 0; k < move.distance; k++)
    {
        //off board
        if(board[currX][currY].compare("-") == 0)
        {
            return false;
        }

        //at start location
        if(currX == x && currY == y)
        {
            //placing less than 0 pieces
            if(move.toLeave[k] < 0)
            {
                return false;
            }
        }
        else
        {
            //placing less than 1 pieces
            if(move.toLeave[k] < 1)
            {
                return false;
            }
        
            if(board[currX][currY].length() > 0)
            {
                //cant be placed on current spot
                top = board[currX][currY].at(board[currX][currY].length() - 1);
                if(top == 'c' || top == 'C' || ((top == 's' || top == 'S') && (current.compare("c") != 0 && current.compare("C") != 0)))
                {
                    return false;
                }
            }
        }

        current = current.substr(move.toLeave[k], current.length() - move.toLeave[k]);
        currX += dx;
        currY += dy;
    }

    //valid move
    return true;
}

int Board::gameDone()
{
    int result = roadDone();
    if(result == 0)
    {
        result = boardFilled();
        if(result == 0)
        {
            return noPieces();
        }
        else
        {
            return result;
        }
    }
    else
    {
        return result;
    }
}

int Board::roadDone()
{
    for(int k = 1; k <= boardSize; k++)
    {
        int result = checkOneRoad(1, k, boardSize, 0);
        if(result != 0)
        {
            return result;
        }

        result = checkOneRoad(k, 1, 0, boardSize);
        if(result != 0)
        {
            return result;
        }
    }

    return 0;
}

int Board::checkOneRoad(int x, int y, int finalX, int finalY)
{
    if(board[x][y].compare("") == 0)
    {
        return 0;
    }

    bool whiteRoad = true;

    char top = board[x][y].at(board[x][y].length() - 1);
    if(top == 'F' || top == 'C')
    {
        whiteRoad = false;
    }
    else if(top == 'f' || top == 'c')
    {
        whiteRoad = true;
    }
    else
    {
        return 0;
    }

    int visited[10][10];
    for(int k = 0; k < 10; k++)
    {
        for(int a = 0; a < 10; a++)
        {
            if(k < 1 || a < 1 || k > boardSize || a > boardSize)
            {
                visited[k][a] = -1;
            }
            else
            {
                visited[k][a] = 0;
            }
        }
    }

    if(roadBFS(visited, x, y, finalX, finalY, whiteRoad) == 1)
    {
        if(whiteRoad)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
}

int Board::roadBFS(int visited[][10], int x, int y, int finalX, int finalY, bool whiteRoad)
{
    if(x == finalX || y == finalY)
    {
        return 1;
    }

    for(int k = -1; k < 2; k++)
    {
        for(int a = -1; a < 2; a++)
        {
            if(((k == -1 || k == 1) && a == 0) || ((a == -1 || a == 1) && k == 0))
            {
                if(board[x+k][y+a].compare("") != 0)
                {
                    char top = board[x+k][y+a].at(board[x+k][y+a].length() - 1);
                    if(visited[x+k][y+a] == 0 && (top == 'f' || top == 'c') == whiteRoad && (top != 's' && top != 'S'))
                    {
                        visited[x+k][y+a] = 1;
                        int result = roadBFS(visited, x + k, y + a, finalX, finalY, whiteRoad);
                        if(result == 1)
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int Board::boardFilled()
{
    for(int k = 1; k <= boardSize; k++)
    {
        for(int a = 1; a < boardSize; a++)
        {
            if(board[k][a].compare("") == 0)
            {
                return 0;
            }
        }
    }

    return greaterLand();
}

int Board::noPieces()
{
    if((whitePieces == 0 && whiteCapstones == 0) || (blackPieces == 0 && blackCapstones == 0))
    {
        return greaterLand();
    }

    return 0;
}

int Board::greaterLand()
{
    int whiteSum = 0;
    int blackSum = 0;

    for(int k = 1; k <= boardSize; k++)
    {
        for(int a = 1; a < boardSize; a++)
        {
            if(board[k][a].compare("") != 0)
            {
                char top = board[k][a].at(board[k][a].length() - 1);
                if(top == 'f' || top == 's' || top == 'c')
                {
                    whiteSum++;
                }
                else
                {
                    whiteSum--;
                }
            }
        }
    }

    if(whiteSum > blackSum)
    {
        return 1;
    }
    else if(blackSum > whiteSum)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

void Board::print()
{
    cout << "White pieces: " << whitePieces << endl;
    cout << "White capstones: " << whiteCapstones << endl;
    cout << "Black pieces: " << blackPieces << endl;
    cout << "Black capstones: " << blackCapstones << endl;
    for(int k = 1; k <= boardSize; k++)
    {
        for(int a = 1; a <= boardSize; a++)
        {
            cout << "---------";
        }
        cout << "-" << endl << "|";
        for(int a = 1; a <= boardSize; a++)
        {
            cout.width(8);
            cout << std::left << board[a][k] << "|";
        }
        cout << endl;
    }
    for(int a = 1; a <= boardSize; a++)
    {
        cout << "---------";
    }
    cout << "-" << endl;
}
