#ifndef CIRCLE_ALGORITHMS_H
#define CIRCLE_ALGORITHMS_H

#define myEpsilon 1e-9

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <float.h>
#include <time.h>

using namespace std;

struct Point
{
    double x, y;
    string toStr()
    {
        return "(" + to_string(x) + ", " + to_string(y) + ")";
    }
    bool isOutside(Point& center, double radius)
    {
        double distance = Point::distance(*this, center);
        return distance > (radius + myEpsilon);
    }
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
    cout << "---------------------\n";
    clock_t start = clock();
    // Initialization ----------------------
    Point *minX, *maxX, *minY, *maxY;
    minX = maxX = minY = maxY = &points[0];
    for(auto& p : points)
    {
        if(p.x < minX->x - myEpsilon) minX = &p;
        if(p.x > maxX->x + myEpsilon) maxX = &p;
        if(p.y < minY->y - myEpsilon) minY = &p;
        if(p.y > maxY->y + myEpsilon) maxY = &p;
    }

    Point center = {(minX->x + maxX->x)/2, (minX->x + maxX->x)/2};
    double radius = (maxX->x - minX->x)/2;

    if ((maxY->y - minY->y) > (maxX->x - minX->x) + myEpsilon)
    {
        center = {(minY->x + maxY->x)/2, (minY->y + maxY->y)/2};
        radius = (maxY->y - minY->y)/2;
    }

    // Update circle ----------------------
    for(auto& p : points)
    {
        if(p.isOutside(center, radius))
        {
            double distance = Point::distance(p, center);
            center.x += ((distance-radius)/2) * (p.x - center.x) / distance;
            center.y += ((distance-radius)/2) * (p.y - center.y) / distance;
            radius = (radius + distance)/2;
            // cout << "Heuristic: New center: " << center.toStr() << " r = " << radius << "\n";
        }
    }
    clock_t end = clock();
    double elapsed = double(end - start)/CLOCKS_PER_SEC;

    // Verification of correctness ----------------------
    int outside = 0;
    for(auto& p : points)
        if(p.isOutside(center, radius)) ++outside;
    if(outside)
        cout << "Heuristic: found " << outside << " points outside.\n";

    cout << "Heuristic results: "
         << "\n\tTime: " << setprecision(5) << elapsed * 1000 << " ms."
         << "\n\tCenter: " << center.toStr()
         << "\n\tRadius: " << radius << "\n";
    cout << "---------------------\n";
    return vector<double> ({center.x, center.y, radius});
}

inline vector<double> defineCircle(Point& p1, Point& p2, Point& p3)
{
    double A = p1.x*(p2.y-p3.y) - p1.y*(p2.x-p3.x) + p2.x*p3.y - p3.x*p2.y;
    double B = (p1.x*p1.x + p1.y*p1.y)*(p3.y-p2.y) + (p2.x*p2.x+p2.y*p2.y)*(p1.y-p3.y)
               + (p3.x*p3.x+p3.y*p3.y)*(p2.y-p1.y);
    double C = (p1.x*p1.x + p1.y*p1.y)*(p2.x-p3.x) + (p2.x*p2.x+p2.y*p2.y)*(p3.x-p1.x)
               + (p3.x*p3.x+p3.y*p3.y)*(p1.x-p2.x);

    double x = -B/(2*A);
    double y = -C/(2*A);

    double radius = sqrt((x-p1.x)*(x-p1.x) + (y-p1.y)*(y-p1.y));

    return {x, y, radius};
}

// returns {xCenter, yCenter, radius} of the minimum circle found
inline vector<double> minCircleWithTwoPoints(vector<Point>& points, int pIdx, int qIdx)
{
    Point center = {(points[pIdx].x + points[qIdx].x)/2, (points[pIdx].x + points[qIdx].x)/2};
    double radius = abs(points[qIdx].x - points[pIdx].x)/2;

    for(int i=0; i<pIdx; ++i)
        if(points[i].isOutside(center, radius))
        {
            vector<double> result = defineCircle(points[i], points[pIdx], points[qIdx]);
            center.x = result[0];
            center.y = result[1];
            radius = result[2];
        }
    return {center.x, center.y, radius};
}

// returns {xCenter, yCenter, radius} of the minimum circle found
inline vector<double> minCircleWithPoint(vector<Point>& points, int qIdx)
{
    Point center = {(points[0].x + points[qIdx].x)/2, (points[0].x + points[qIdx].x)/2};
    double radius = abs(points[qIdx].x - points[0].x)/2;

    for(int i=1; i<qIdx; ++i)
        if(points[i].isOutside(center, radius))
        {
            vector<double> result = minCircleWithTwoPoints(points, i, qIdx);
            center.x = result[0];
            center.y = result[1];
            radius = result[2];
        }
    return {center.x, center.y, radius};
}

// returns {xCenter, yCenter, radius} of the minimum circle found
inline vector<double> minCircle(vector<Point> points)
{
    cout << "---------------------\n";
    clock_t start = clock();
    // Initialization ----------------------
    // shuffle points
    int n = points.size();
    for (int i = n-1; i > 0; --i)
    {
        int j = rand() % (i+1);
        swap(points[i], points[j]);
    }

    Point center = {(points[0].x + points[1].x)/2, (points[0].x + points[1].x)/2};
    double radius = abs(points[1].x - points[0].x)/2;

    // Update circle ----------------------
    for(int i=2; i<points.size(); ++i)
        if(points[i].isOutside(center, radius))
        {
            vector<double> result = minCircleWithPoint(points, i);
            center.x = result[0];
            center.y = result[1];
            radius = result[2];
        }

    clock_t end = clock();
    double elapsed = double(end - start)/CLOCKS_PER_SEC;

   // Verification of correctness ----------------------
    int outside = 0;
    for(auto& p : points)
        if(p.isOutside(center, radius)) ++outside;
    if(outside)
        cout << "Exact method: found " << outside << " points outside.\n";

    cout << "Exact method results: "
         << "\n\tTime: " << setprecision(5) << elapsed * 1000 << " ms."
         << "\n\tCenter: " << center.toStr()
         << "\n\tRadius: " << radius << "\n";
    cout << "---------------------\n";

    return vector<double> ({center.x, center.y, radius});
}

#endif