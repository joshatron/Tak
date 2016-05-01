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
    int x = move.x + 1;
    int y = move.y + 1;
    if(move.type.compare("place") == 0)
    {
        if(board[x][y].compare("") == 0)
        {
            if((whiteMove && firstMove == 0) || (!whiteMove && firstMove > 0))
            {
                if(move.placeType == 0)
                {
                    board[x][y] = "f";
                }
                else if(move.placeType == 1)
                {
                    board[x][y] = "s";
                }
                else
                {
                    board[x][y] = "c";
                }
            }
            else
            {
                if(move.placeType == 0)
                {
                    board[x][y] = "F";
                }
                else if(move.placeType == 1)
                {
                    board[x][y] = "S";
                }
                else
                {
                    board[x][y] = "C";
                }
            }

            if(firstMove > 0)
            {
                firstMove--;
            }
            whiteMove = !whiteMove;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(board[x][y].length() > 0)
        {
            bool white = true;

            if(board[x][y].at(board[x][y].length() - 1) == 'F' || board[x][y].at(board[x][y].length() - 1) == 'S' ||
               board[x][y].at(board[x][y].length() - 1) == 'C')
            {
                white = false;
            }

            if(((whiteMove && firstMove == 0) || (!whiteMove && firstMove > 0)) == white)
            {
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
                string drop[8];
                string current = board[x][y];
                for(int k = 0; k < move.distance; k++)
                {
                    if(board[currX][currY].compare("-") == 0)
                    {
                        return false;
                    }
                    if(move.toLeave[k] > current.length())
                    {
                        return false;
                    }
                    if((currX != x || currY != y) && move.toLeave[k] == 0)
                    {
                        return false;
                    }
                    drop[k] = current.substr(0, move.toLeave[k]);
                    current = current.substr(move.toLeave[k], current.length() - move.toLeave[k]);

                    if(board[currX][currY].length() > 0)
                    {
                        char top = board[currX][currY].at(board[currX][currY].length() - 1);
                        if(top == 'c' || top == 'C' || ((top == 's' || top == 'S') && (drop[k].compare("c") != 0 && drop[k].compare("C") != 0)))
                        {
                            return false;
                        }
                    }
                    currX += dx;
                    currY += dy;
                }

                if(current.length() > 0)
                {
                    return false;
                }

                board[x][y] = "";
                currX = x;
                currY = y;
                for(int k = 0; k < move.distance; k++)
                {
                    if(board[currX][currY].length() > 0)
                    {
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
                    board[currX][currY] = board[currX][currY].append(drop[k]);
                    currX += dx;
                    currY += dy;
                }

                if(firstMove > 0)
                {
                    firstMove--;
                }
                whiteMove = !whiteMove;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    return false;
}

int Board::gameDone()
{
    for(int k = 1; k <= boardSize; k++)
    {
        if(board[1][k].length() == 0 || board[k][1].length() == 0)
        {
            continue;
        }

        bool horizontal = true;
        char horizontalChar = board[1][k].at(board[1][k].length() - 1);
        bool horizontalWhite = (horizontalChar == 'f' || horizontalChar == 's' || horizontalChar == 'c');
        bool vertical = true;
        char verticalChar = board[k][1].at(board[k][1].length() - 1);
        bool verticalWhite = (verticalChar == 'f' || verticalChar == 's' || verticalChar == 'c');

        if(verticalChar == 's' || verticalChar == 'S' || horizontalChar == 's' || horizontalChar == 'S')
        {
            continue;
        }

        for(int a = 2; a <= boardSize; a++)
        {
            if(board[a][k].length() == 0)
            {
                horizontal = false;
            }
            else
            {
                char temp = board[a][k].at(board[a][k].length() - 1);
                if(temp == 's' || temp == 'S')
                {
                    horizontal = false;
                }
                else if(horizontalWhite != (temp == 'f' || temp == 'c'))
                {
                    horizontal = false;
                }
            }

            if(board[k][a].length() == 0 || board[k][a].at(board[k][a].length() - 1) != verticalChar)
            {
                vertical = false;
            }
            else
            {
                char temp = board[k][a].at(board[a][k].length() - 1);
                if(temp == 's' || temp == 'S')
                {
                    vertical = false;
                }
                else if(verticalWhite != (temp == 'f' || temp == 'c'))
                {
                    vertical = false;
                }
            }
        }

        if(horizontal)
        {
            if(horizontalWhite)
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }
        if(vertical)
        {
            if(verticalWhite)
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }
    }

    return 0;
}

void Board::print()
{
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
