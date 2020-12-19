#pragma once
#include <string>

using namespace std;

class Point
{
public:
    double x, y;
    int id;
    string toStr();
    static double distance(Point& p1, Point& p2);
    Point operator-(Point& other);
    Point operator+(Point& other);
    Point operator*(double a);
    Point operator/(double a);
    bool operator==(Point& other);
    Point normalize();
    double magnitude();
    double dot(Point& other);
};