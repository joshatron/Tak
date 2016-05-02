#include <iostream>
#include "takengine.h"
#include "move.h"

using std::cout;
using std::cin;
using std::endl;

int main()
{
    TakEngine engine = TakEngine(5, true);
    while(engine.gameDone() == 0)
    {
        engine.printBoard();
        if(engine.whiteTurn())
        {
            cout << "White's turn" << endl;
        }
        else
        {
            cout << "Black's turn" << endl;
        }
        int choice;
        cout << "Place or move?[0-1]: ";
        cin >> choice;

        if(choice == 0)
        {
            int x;
            int y;
            int type;
            cout << "X position: ";
            cin >> x;
            cout << "Y position: ";
            cin >> y;
            string typeStr;
            cout << "Type[fsc]: ";
            cin >> typeStr;
            if(typeStr.compare("f") == 0)
            {
                type = 0;
            }
            else if(typeStr.compare("s") == 0)
            {
                type = 1;
            }
            else
            {
                type = 2;
            }
            Move place;
            place.type = "place";
            place.x = x;
            place.y = y;
            place.placeType = type;
            bool result = engine.tryMove(place);
            if(!result)
            {
                cout << "Invalid command" << endl;
            }
        }
        else
        {
            int x;
            int y;
            string dir;
            cout << "X position: ";
            cin >> x;
            cout << "Y position: ";
            cin >> y;
            cout << "Direction[nsew]: ";
            cin >> dir;
            int num = engine.numberAtSpot(x, y);
            int drop[8];
            int distance = 0;
            Move move;
            while(num > 0)
            {
                cout << "Drop how many at position " << distance << ": ";
                cin >> move.toLeave[distance];
                num -= move.toLeave[distance];
                distance++;
            }

            move.type = "move";
            move.x = x;
            move.y = y;
            move.direction = dir;
            move.distance = distance;
            bool result = engine.tryMove(move);
            if(!result)
            {
                cout << "Invalid command" << endl;
            }
        }
    }

    if(engine.gameDone() == 1)
    {
        cout << "White wins!" << endl;
    }
    else
    {
        cout << "Black wins!" << endl;
    }
    engine.printBoard();
}
