#pragma once
#include "point.h"

class Circle
{
public:
    Point center;
    double radius;
    Circle(Point& c, double r);
    Circle(Point& p1, Point& p2);
    Circle(Point& p1, Point& p2, Point& p3);
    bool notContain(Point& p);
};