#include "element.h"

Element::Element(vec2 c, double w, double h, int hType, vec3 l, vec3 d, vec3 lh, vec3 dh)
{
    center = c;
    width = w;
    height = h;
    highlightType = hType;
    lightColor = l;
    darkColor = d
    lightHighlightColor = lh;
    darkHighlightColor = dh;
    mouseLoc = vec2(center.x - width, center.y - height);
    mouseIn = false;
}

void Element::setMouseLoc(vec2 loc)
{
    mouseLoc = vec2(loc.x - center.x, loc.y - center.y);
    if(mouseLoc.x < center.x - width / 2 || mouseLoc.x > center.x + width / 2 ||
       mouseLoc.y < center.y - height / 2 || mouseLoc.y > center.y + height / 2)
    {
        mouseIn = false;
    }
    else
    {
        mouseIn = true;
    }
}

void Element::setHighlightType(int type)
{
    highlightType = type;
}

void Element::draw(Screen& game)
{
}
