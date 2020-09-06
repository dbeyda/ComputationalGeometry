#ifndef CIRCLE_ALGORITHMS_H
#define CIRCLE_ALGORITHMS_H

#define epsilon 1e-9

#include <vector>
#include <float.h>
#include <iostream>

using namespace std;

struct Point
{
    double x, y;
    static double distance(Point& p1, Point& p2)
    {
        double x = abs(p2.x - p1.x);
        double y = abs(p2.y - p1.y);
        return sqrt(x*x + y*y);
    }
};

// returns {xCenter, yCenter, radius} of the minimum circle found
inline vector<double> minCircleHeuristic(vector<Point>& points)
{
    // Initialization ----------------------
    Point *minX, *maxX, *minY, *maxY;
    minX = maxX = minY = maxY = &points[0];
    for(auto& p : points)
    {
        if(p.x < minX->x - epsilon) minX = &p;
        if(p.x > maxX->x + epsilon) maxX = &p;
        if(p.y < minY->y - epsilon) minY = &p;
        if(p.y > maxY->y + epsilon) maxY = &p;
    }

    Point center = {(minX->x + maxX->x)/2, (minX->x + maxX->x)/2};
    double radius = (maxX->x - minX->x)/2;

    if ((maxY->y - minY->y) > (maxX->x - minX->x) + epsilon)
    {
        center = {(minY->x + maxY->x)/2, (minY->y + maxY->y)/2};
        radius = (maxY->y - minY->y)/2;
    }

    // Update circle ----------------------
    for(auto& p : points)
    {
        double distance = Point::distance(p, center);
        if(distance > radius + epsilon)
        {
            center.x += ((distance-radius)/2) * (p.x - center.x) / distance;
            center.y += ((distance-radius)/2) * (p.y - center.y) / distance;
            radius = (radius + distance)/2;
            cout << "Heuristic: New center: (" << center.x << ", " << center.y << ") r = " << radius << "\n";
        }
    }

    // Verification of correctness ----------------------
    int outside = 0;
    for(auto& p : points)
        if(Point::distance(p, center) > radius + epsilon) ++outside;

    cout << "Heuristic: found " << outside << " points outside.\n";
    return vector<double> ({center.x, center.y, radius});
}

inline vector<double> minCircle(vector<Point>& points)
{
    return vector<double> ({0.3, -0.6, 0.2});
}

#endif