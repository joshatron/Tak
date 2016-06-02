#ifndef ELEMENT_H
#define ELEMENT_H

class Element
{
    public:
        vec2 center;
        double width;
        double height;

        Element(vec2 c, double w, double h, int hType, vec3 l, vec3 d, vec3 lh, vec3 dh);
        //loc direct mouse coordinates
        void setMouseLoc(vec2 loc);
        void setHighlightType(int type);
        void draw(Screen& game);
    private:
        int highlightType;
        vec2 mouseLoc;
        bool mouseIn;
        vec3 lightColor;
        vec3 lightHighlightColor;
        vec3 darkColor;
        vec3 darkHighlightColor;
};

#endif
