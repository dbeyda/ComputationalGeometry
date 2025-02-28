#include <cmath>
#include "point.h"

using namespace std;

string Point::toStr()
{
    return "(" + to_string(x) + ", " + to_string(y) + ")";
}

double Point::distance(Point& p1, Point& p2)
{
    double x = abs(p2.x - p1.x);
    double y = abs(p2.y - p1.y);
    return sqrt(x*x + y*y);
}