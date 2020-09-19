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


struct Vertex
{
    Point* point;
    int id;
};

/**
 * A function that returns the orientation determinant
 * of a point p with respect to a segment ab
 */
inline double orientation(Point& p, Point& a, Point& b)
{
    return (b.x*p.y + a.x*b.y + a.y*p.x
            - a.y*b.x - a.x*p.y - b.y*p.x);
}

// // find first vertex (rightmost)
// inline int findFirstVertex(list<Point> )
// {
//     list<Vertex>::iterator firstVertex = vertices.begin();
//     for(auto it = vertices.begin(); it != vertices.end(); ++it)
//         if (it->point->x > firstVertex->point->x + myEpsilon) firstVertex = it;
//     return firstVertex;
// }

/**
 * calculates angle between p2 and p3, using p1 as origin
 * ranging from 0 to 2 pi
 */
// inline double getAngle(Point& p1, Point& p2, Point& p3)
// {
//     double angle2 = atan2(p2.y - p1.y, p2.x - p1.x);
//     double angle3 = atan2(p3.y - p1.y, p3.x - p1.x);
//     if (angle2 < 0 - myEpsilon) angle2 += M_PI * 2;
//     if (angle3 < 0 - myEpsilon) angle3 += M_PI * 2;
//     cout << angle3 - angle2 << endl;
//     return angle3 - angle2;
// }
inline double getAngle(Point& p1, Point& p2, Point& p3)
{
   Point v2 = (p2 - p1);
   v2 = v2 / sqrt(v2.x*v2.x + v2.y*v2.y);
   Point v3 = (p3 - p1);
   v3 = v3 / sqrt(v3.x*v3.x + v3.y*v3.y);
    return acos(v2.x*v3.x + v2.y * v3.y);
}


inline void findConvexHull(vector<Point>& points, vector<Point>& hull, vector<int>& ids)
{
    // finding first hull point
    int firstIdx = 0;
    for(int i=0; i<points.size(); ++i)
        if (points[i].x > points[firstIdx].x + myEpsilon) firstIdx = i;

    hull.push_back(points[firstIdx]);
    ids.push_back(firstIdx);

    // finding second hull point
    int secondIdx = 0;
    double minAngle = DBL_MAX;
    Point xRef = points[firstIdx];
    xRef.x += 5;
    for(int i=0; i<points.size(); ++i)
    {
        if (i == firstIdx) continue;
        double angle = getAngle(points[firstIdx], xRef, points[i]);
        if (angle < minAngle - myEpsilon)
        {
            secondIdx = i;
            minAngle = angle;
        }
    }
    hull.push_back(points[secondIdx]);
    ids.push_back(secondIdx);

    // find other points
    int lastIdx = firstIdx;
    int originIdx = secondIdx;
    while(originIdx != firstIdx)
    {
        // find next point
        double maxAngle = 0l;
        int maxIdx = 0;
        if(hull.size() == 9) break;
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

        // add point to solution
        hull.push_back(points[maxIdx]);
        ids.push_back(maxIdx);
        // update indexes
        lastIdx = originIdx;
        originIdx = maxIdx;
    }
}
