#ifndef MOVE_H
#define MOVE_H

#include <string>

using std::string;

class Move
{
    public:
        string type;
        int x;
        int y;
        //0- flat stone
        //1- standing stone
        //2- capstone
        int placeType;
        string direction;
        int toLeave[8];
        int distance;
};

#endif
