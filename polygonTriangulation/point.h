#pragma once
#include <string>

using namespace std;

class Point
{
public:
    double x, y;
    string toStr();
    static double distance(Point& p1, Point& p2);
};
