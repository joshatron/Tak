#include "screen.h"

Screen::Screen()
{
}

void Screen::addPoly(vector<vec2> points, vec3 color, bool outline, vec3 outlineColor)
{
    for(int k = 2; k < points.size(); k++)
    {
        polyPoints.push_back(points.at(0));
        polyPoints.push_back(points.at(k-1));
        polyPoints.push_back(points.at(k));

        polyColors.push_back(color);
        polyColors.push_back(color);
        polyColors.push_back(color);
    }

    if(outline)
    {
        for(int k = 1; k < points.size(); k++)
        {
            linePoints.push_back(points.at(k-1));
            linePoints.push_back(points.at(k));

            lineColors.push_back(outlineColor);
            lineColors.push_back(outlineColor);
        }
    }
}

void Screen::addLine(vec2 start, vec2 end, vec3 color)
{
    linePoints.push_back(start);
    linePoints.push_back(end);

    lineColors.push_back(color);
    lineColors.push_back(color);
}

void Screen::reset()
{
    polyPoints.clear();
    polyColors.clear();
    linePoints.clear();
    lineColors.clear();
}
