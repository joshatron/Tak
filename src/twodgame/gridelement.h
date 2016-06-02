#ifndef GRID_ELEMENT_H
#define GRID_ELEMENT_H

#include "element.h"

class GridElement: public Element
{
    GridElement(vec2 c, double w, double h, int hType, vec3 l, vec3 d, vec3 lh, vec3 dh, int gw);
};

#endif
