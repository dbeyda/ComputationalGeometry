#define myEpsilon 1e-9

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <float.h>
#include <time.h>
#include "circle.h"
#include "point.h"

using namespace std;


// returns {xCenter, yCenter, radius} of the minimum circle found
inline Circle minCircleHeuristic(vector<Point>& points)
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

    Circle circle(*minX, *maxX);
    if ((maxY->y - minY->y) > (maxX->x - minX->x) + myEpsilon)
        circle = Circle(*minY, *maxY);

    // Update circle ----------------------
    for(auto& p : points)
    {
        if(circle.notContain(p))
        {
            double distance = Point::distance(p, circle.center);
            circle.center.x += ((distance-circle.radius)/2) * (p.x - circle.center.x) / distance;
            circle.center.y += ((distance-circle.radius)/2) * (p.y - circle.center.y) / distance;
            circle.radius = (circle.radius + distance)/2;
            // cout << "Heuristic: New center: " << center.toStr() << " r = " << radius << "\n";
        }
    }
    clock_t end = clock();
    double elapsed = double(end - start)/CLOCKS_PER_SEC;

    // Verification of correctness ----------------------
    int outside = 0;
    for(auto& p : points)
        if(circle.notContain(p)) ++outside;
    if(outside)
        cout << "Heuristic: found " << outside << " points outside.\n";

    cout << "Heuristic results: "
         << "\n\tTime: " << setprecision(5) << elapsed * 1000 << " ms."
         << "\n\tCenter: " << circle.center.toStr()
         << "\n\tRadius: " << circle.radius << "\n";
    cout << "---------------------\n";
    return circle;
}

// returns {xCenter, yCenter, radius} of the minimum circle found
inline Circle minCircleWithTwoPoints(vector<Point>& points, int pIdx, int qIdx)
{
    Circle circle(points[qIdx], points[pIdx]);

    for(int i=0; i<pIdx; ++i)
        if(circle.notContain(points[i]))
            circle = Circle(points[i], points[pIdx], points[qIdx]);
    return circle;
}

// returns {xCenter, yCenter, radius} of the minimum circle found
inline Circle minCircleWithPoint(vector<Point>& points, int qIdx)
{
    Circle circle(points[0], points[qIdx]);
    for(int i=1; i<qIdx; ++i)
        if(circle.notContain(points[i]))
            circle = minCircleWithTwoPoints(points, i, qIdx);
    return circle;
}

// returns {xCenter, yCenter, radius} of the minimum circle found
inline Circle minCircle(vector<Point> points)
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

    Circle circle(points[0], points[1]);

    // Update circle ----------------------
    for(int i=2; i<points.size(); ++i)
        if(circle.notContain(points[i]))
            circle = minCircleWithPoint(points, i);

    clock_t end = clock();
    double elapsed = double(end - start)/CLOCKS_PER_SEC;

   // Verification of correctness ----------------------
    int outside = 0;
    for(auto& p : points)
        if(circle.notContain(p)) ++outside;
    if(outside)
        cout << "Exact method: found " << outside << " points outside.\n";

    cout << "Exact method results: "
         << "\n\tTime: " << setprecision(5) << elapsed * 1000 << " ms."
         << "\n\tCenter: " << circle.center.toStr()
         << "\n\tRadius: " << circle.radius << "\n";
    cout << "---------------------\n";

    return circle;
}
