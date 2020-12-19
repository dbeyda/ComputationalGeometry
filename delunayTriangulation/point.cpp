#include <cmath>
#include "point.h"

#define myEpsilon 1e-9

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

Point Point::operator-(Point& other)
{
    return Point({x - other.x, y - other.y});
}

Point Point::operator+(Point& other)
{
    return Point({x + other.x, y + other.y});
}

Point Point::operator*(double a)
{
    return Point({x*a, y*a});
}

Point Point::operator/(double a)
{
    return Point({x/a, y/a});
}

bool Point::operator==(Point& other)
{
    return abs(x - other.x) < myEpsilon && abs(y - other.y) < myEpsilon;
}

Point Point::normalize()
{
    return Point({x/magnitude(), y/magnitude()});
}

double Point::magnitude()
{
    return sqrt(x*x + y*y);
}

double Point::dot(Point& other)
{
    return x*other.x + y*other.y;
}