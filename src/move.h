#ifndef MOVE_H
#define MOVE_H

#include <string>

using std::string;

//hold information about a move
class Move
{
    public:
        //independant of move type

        //"place" or "move"
        string type;
        //starts at 0
        int x;
        int y;

        //for place type

        //0- flat stone
        //1- standing stone
        //2- capstone
        int placeType;

        //for move type

        //n- up
        //s- down
        //e- right
        //w- left
        string direction;
        //number of peices to leave at each spot
        int toLeave[8];
        //how far the stack moves
        int distance;
};

#endif
