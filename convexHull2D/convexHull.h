#define myEpsilon 1e-9

#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <random>
#include <float.h>
#include <time.h>
#include "point.h"

using namespace std;

/**
 * calculates the angle between two vectors
 * in the range 0 to pi
 */
inline double getAngle(Point& p1, Point& p2, Point& p3)
{
    Point v2 = (p2 - p1).normalize();
    Point v3 = (p3 - p1).normalize();
    return acos(v2.dot(v3));
}

/**
 * Finds convex hull of a set of points
 * adds hull points indexes to ids vector
 */
inline void findConvexHull(vector<Point>& points, vector<int>& ids)
{
    // finding first hull point -- rightmost point
    int firstIdx = 0;
    for(int i=0; i<points.size(); ++i)
        if (points[i].x > points[firstIdx].x + myEpsilon) firstIdx = i;

    ids.push_back(firstIdx);

    // finding second hull point -- minimizing angle to axis x+
    int secondIdx = 0;
    double minAngle = DBL_MAX;
    Point xRef = points[firstIdx];
    xRef.x += 5;
    for(int i=0; i<points.size(); ++i)
    {
        if (i == firstIdx) continue;
        double angle = getAngle(points[firstIdx], points[i], xRef);
        if (angle < minAngle - myEpsilon)
        {
            secondIdx = i;
            minAngle = angle;
        }
    }
    ids.push_back(secondIdx);

    // find other points
    int lastIdx = firstIdx;
    int originIdx = secondIdx;
    while(true)
    {
        // find next point
        double maxAngle = 0l;
        int maxIdx = 0;
        for(int i=0; i < points.size(); ++i)
        {
            if(i == lastIdx || i == originIdx) continue;
            double angle = abs(getAngle(points[originIdx], points[lastIdx], points[i]));
            if(angle > maxAngle + myEpsilon)
            {
                maxIdx = i;
                maxAngle = angle;
            }
        }
        if (maxIdx == firstIdx) break;

        ids.push_back(maxIdx);
        lastIdx = originIdx;
        originIdx = maxIdx;
    }
}
