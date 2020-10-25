#include "triangleAdjacencyTable.h"
#include <vector>
#include <math.h>

#define myEpsilon 1e-9


TriangleAdjacencyTable::TriangleAdjacencyTable()
        : table(vector<AdjacencyElement>()) {};

int TriangleAdjacencyTable::findContainingTriangle(Point& p)
{
    // TODO: implement this as a topological walk
    // TODO: consider case where point is at a triangle edge
    for(int i=0; i<table.size(); ++i)
        if (pointInsideTriangle(p, table[i].p[0], table[i].p[1], table[i].p[2]))
            return i;
    return -1;
};

double TriangleAdjacencyTable::orientation(Point& p, Point& a, Point& b)
{
    double value = (b.x*p.y + a.x*b.y + a.y*p.x
            - a.y*b.x - a.x*p.y - b.y*p.x);
    return value;
}

bool TriangleAdjacencyTable::pointInsideCircle(Point& p, Point& a, Point& b, Point& c)
{
    // find center and radius of circle defined by 3 points
    double A = a.x*(b.y-c.y) - a.y*(b.x-c.x) + b.x*c.y - c.x*b.y;
    double B = (a.x*a.x + a.y*a.y)*(c.y-b.y) + (b.x*b.x+b.y*b.y)*(a.y-c.y)
            + (c.x*c.x+c.y*c.y)*(b.y-a.y);
    double C = (a.x*a.x + a.y*a.y)*(b.x-c.x) + (b.x*b.x+b.y*b.y)*(c.x-a.x)
            + (c.x*c.x+c.y*c.y)*(a.x-b.x);

    double x = -B/(2*A);
    double y = -C/(2*A);

    Point center = Point({x, y});
    double radius = sqrt((x-a.x)*(x-a.x) + (y-a.y)*(y-a.y));

    // return if point is inside circle
    double distance = Point::distance(center, p);
    return distance > (radius + myEpsilon);
}

bool TriangleAdjacencyTable::isConvex(Point& p, Point& a, Point& b)
{
    return orientation(p, a, b) > myEpsilon;

    // this is not ideal, but was needed for the star triangulation
    // to be correct
    // return orientation(p, a, b) > 200;
}

/**
 * A function that checks if a point p is inside a
 * triangle ABC, where the sequence of points A, B and C
 * follows the counter-clockwise orientation
 */
bool TriangleAdjacencyTable::pointInsideTriangle(Point& p, Point& a, Point& b, Point& c)
{
    if (orientation(p, a, b) <= -myEpsilon) return false;
    if (orientation(p, b, c) <= -myEpsilon) return false;
    if (orientation(p, c, a) <= -myEpsilon) return false;
    return true;
}

