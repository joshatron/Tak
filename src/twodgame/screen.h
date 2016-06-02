#ifndef SCREEN_H
#define SCREEN_H

class Screen
{
    public:
        Screen();
        void addPoly(vector<vec2> points, vec3 color, bool outline, vec3 outlineColor);
        void addLine(vec2 start, vec2 end, vec3 color);
        void reset();

        vector<vec2> polyPoints;
        vector<vec3> polyColors;

        vector<vec2> linePoints;
        vector<vec3> lineColors;
    private:
};

#endif
