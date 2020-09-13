#include <cmath>
#include "circle.h"
#define myEpsilon 1e-9

Circle::Circle(Point& c, double r) : center(c), radius(r) {};

Circle::Circle(Point& p1, Point& p2)
{
    center = Point({(p1.x + p2.x)/2, (p1.y + p2.y)/2});
    radius = Point::distance(p1, p2)/2;
};

Circle::Circle(Point& p1, Point& p2, Point& p3)
{
    double A = p1.x*(p2.y-p3.y) - p1.y*(p2.x-p3.x) + p2.x*p3.y - p3.x*p2.y;
    double B = (p1.x*p1.x + p1.y*p1.y)*(p3.y-p2.y) + (p2.x*p2.x+p2.y*p2.y)*(p1.y-p3.y)
            + (p3.x*p3.x+p3.y*p3.y)*(p2.y-p1.y);
    double C = (p1.x*p1.x + p1.y*p1.y)*(p2.x-p3.x) + (p2.x*p2.x+p2.y*p2.y)*(p3.x-p1.x)
            + (p3.x*p3.x+p3.y*p3.y)*(p1.x-p2.x);

    double x = -B/(2*A);
    double y = -C/(2*A);

    center = Point({x, y});
    radius = sqrt((x-p1.x)*(x-p1.x) + (y-p1.y)*(y-p1.y));
};

bool Circle::notContain(Point& p)
{
    double distance = Point::distance(center, p);
    return distance > (radius + myEpsilon);
}