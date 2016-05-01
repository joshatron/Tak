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
        cout << "1" << endl;
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
            cout << "X position: ";
            cin >> x;
            cout << "Y position: ";
            cin >> y;
            Move place;
            place.type = "place";
            place.x = x;
            place.y = y;
            cout << "1.1" << endl;
            bool result = engine.tryMove(place);
            cout << "1.2" << endl;
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
        }
        cout << "2" << endl;
    }

    if(engine.gameDone() == 1)
    {
        cout << "White wins!" << endl;
    }
    else
    {
        couv << "Black wins!" << endl;
    }
}
