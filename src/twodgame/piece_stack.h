#ifndef PIECE_STACK_H
#define PIECE_STACK_H

#include <string>
#include "element.h"

class PieceStack: public Element
{
    public:
        PieceStack(vec2 c, double w, double h, int hType, vec3 l, vec3 d, vec3 lh, vec3 dh, string s);
        void draw(Screen& game);
    private:
        string stack;
};

#endif
