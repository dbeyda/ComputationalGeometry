#define myEpsilon 1e-9

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <float.h>
#include <time.h>
#include "point.h"
#include "triangleAdjacencyTable.h"

using namespace std;


double orientation(Point& p, Point& a, Point& b)
{
    return (b.x*p.y + a.x*b.y + a.y*p.x
            - a.y*b.x - a.x*p.y - b.y*p.x);
}

bool isConvex(Point& p, Point& a, Point& b)
{
    return orientation(p, a, b) > myEpsilon;

    // this is not ideal, but was needed for the star triangulation
    // to be correct
    // return orientation(p, a, b) > 200;
}

bool pointInsideCircle(Point& p, Point& a, Point& b, Point& c)
{
    // find center and radius of circle defined by 3 points
    double A = a.x*(b.y-c.y) - a.y*(b.x-c.x) + b.x*c.y - c.x*b.y;
    double B = (a.x*a.x + a.y*a.y)*(c.y-b.y) + (b.x*b.x+b.y*b.y)*(a.y-c.y)
            + (c.x*c.x+c.y*c.y)*(b.y-a.y);
    double C = (a.x*a.x + a.y*a.y)*(b.x-c.x) + (b.x*b.x+b.y*b.y)*(c.x-a.x)
            + (c.x*c.x+c.y*c.y)*(a.x-b.x);

    double x = -B/(2*A);
    double y = -C/(2*A);

    Point center = Point({x, y});
    double radius = sqrt((x-a.x)*(x-a.x) + (y-a.y)*(y-a.y));

    // return if point is inside circle
    double distance = Point::distance(center, p);
    return distance < (radius - myEpsilon);
}


vector<Point> buildWrappingTriangle(vector<Point>& points, TriangleAdjacencyTable& adj)
{
    // get min x, max x
    // get min y, max y
    double minX, maxX, minY, maxY;
    minX = maxX = points.front().x;
    minY = maxY = points.front().y;
    Point center = {0., 0.};

    for(auto it = points.begin(); it != points.end(); it++)
    {
        if(it->x < minX) minX = it->x;
        if(it->x > maxX) maxX = it->x;
        if(it->y < minY) minY = it->y;
        if(it->y > maxY) maxY = it->y;
        center = center + *it;
    }
    center = center / points.size();

    // define largest side and center for the square
    double m;
    if (maxX - minX > maxY - minY)
        m = (maxX - minX)/2;
    else
        m = (maxY - minY)/2;

    adj.table.push_back(
        {
            Point({-30*m, -30*m}) + center,
            Point({30*m, 0}) + center,
            Point({0, 30*m}) + center,
        });

    return {
            Point({-30*m, -30*m}) + center,
            Point({30*m, 0}) + center,
            Point({0, 30*m}) + center,
        };
}

inline int getNextIdx(int i)
{
    return (i+1)%3;
}

inline int getPrevIdx(int i)
{
    return (i-1+3)%3;
}

/**
 * A function that verifies is edge in triangle t
 * opposed to vertex p is a legal edge.
 * If its not, a flip operation is executed.
 */
bool checkAndFlip(int t, int pIdx, TriangleAdjacencyTable& adj)
{
    AdjacencyElement triangle = adj.table[t];
    int oppositeTriangleIdx = triangle.t[pIdx];
    if(oppositeTriangleIdx < 0)
        return false;

    AdjacencyElement oppositeTriangle = adj.table[oppositeTriangleIdx];
    int oppositePointIdx = oppositeTriangle.getTriangleIdx(t);
    Point& oppositePoint = oppositeTriangle.p[oppositePointIdx];

    // flip not needed
    if (!pointInsideCircle(oppositePoint, triangle.p[0], triangle.p[1], triangle.p[2]))
        return false;
    if (!isConvex(triangle.p[pIdx], triangle.p[getNextIdx(pIdx)], oppositePoint))
        return false;
    if (!isConvex(oppositePoint, triangle.p[getPrevIdx(pIdx)], triangle.p[pIdx]))
        return false;

    // flip needed
    // at this point, triangle and opposite triangle are safe copies of the original elements
    int a1Idx = t;
    AdjacencyElement& a1 = adj.table[a1Idx];
    a1 = {triangle.p[pIdx], oppositePoint, triangle.p[getPrevIdx(pIdx)], -1, -1, -1};
    int a2Idx = oppositeTriangleIdx;
    AdjacencyElement& a2 = adj.table[a2Idx];
    a2 = {triangle.p[pIdx], triangle.p[getNextIdx(pIdx)], oppositePoint, -1, -1, -1};

    int t1Idx = triangle.t[getNextIdx(pIdx)];
    int t2Idx = oppositeTriangle.t[getPrevIdx(oppositePointIdx)];
    int t3Idx = oppositeTriangle.t[getNextIdx(oppositePointIdx)];
    int t4Idx = triangle.t[getPrevIdx(pIdx)];

    AdjacencyElement* t1 = t1Idx >= 0 ? &adj.table[t1Idx] : nullptr;
    AdjacencyElement* t2 = t2Idx >= 0 ? &adj.table[t2Idx] : nullptr;
    AdjacencyElement* t3 = t3Idx >= 0 ? &adj.table[t3Idx] : nullptr;
    AdjacencyElement* t4 = t4Idx >= 0 ? &adj.table[t4Idx] : nullptr;

    if (t1)
    {
        t1->t[t1->getTriangleIdx(t)] = a1Idx;
        a1.t[1] = t1Idx;
    }
    if (t2)
    {
        t2->t[t2->getTriangleIdx(oppositeTriangleIdx)] = a1Idx;
        a1.t[0] = t2Idx;
    }
    if (t3)
    {
        t3->t[t3->getTriangleIdx(oppositeTriangleIdx)] = a2Idx;
        a2.t[0] = t3Idx;
    }
    if (t4)
    {
        t4->t[t4->getTriangleIdx(t)] = a2Idx;
        a2.t[2] = t4Idx;
    }

    a1.t[2] = a2Idx;
    a2.t[1] = a1Idx;
    checkAndFlip(a1Idx, 0, adj);
    checkAndFlip(a1Idx, 1, adj);
    checkAndFlip(a1Idx, 2, adj);
    checkAndFlip(a2Idx, 0, adj);
    checkAndFlip(a2Idx, 1, adj);
    checkAndFlip(a2Idx, 2, adj);

    return true;
}

void divideTriangle(int t, Point& p, TriangleAdjacencyTable& adj)
{
    AdjacencyElement triangle = adj.table[t];

    // triangle will be substituted by a1
    adj.table[t] = {p, triangle.p[1], triangle.p[2]};
    // a2 and a3 will be will elements in the list
    adj.table.push_back({p, triangle.p[2], triangle.p[0]});
    adj.table.push_back({p, triangle.p[0], triangle.p[1]});

    int idxA1 = t;
    int idxA2 = adj.table.size()-2;
    int idxA3 = adj.table.size()-1;

    auto& a1 = adj.table[idxA1];
    auto& a2 = adj.table[idxA2];
    auto& a3 = adj.table[idxA3];

    // setting adjacents for a1, a2, a3
    a1.t[0] = triangle.t[0];
    a1.t[1] = idxA2;
    a1.t[2] = idxA3;

    a2.t[0] = triangle.t[1];
    a2.t[1] = idxA3;
    a2.t[2] = idxA1;

    a3.t[0] = triangle.t[2];
    a3.t[1] = idxA1;
    a3.t[2] = idxA2;

    // fixing adjacent triangles
    if (triangle.t[0] >= 0)
    {
        int ref1 = adj.table[triangle.t[0]].getTriangleIdx(t);
        adj.table[triangle.t[0]].t[ref1] = idxA1;
    }
    if (triangle.t[1] >= 0)
    {
        int ref2 = adj.table[triangle.t[1]].getTriangleIdx(t);
        adj.table[triangle.t[1]].t[ref2] = idxA2;
    }
    if (triangle.t[2] >= 0)
    {
        int ref3 = adj.table[triangle.t[2]].getTriangleIdx(t);
        adj.table[triangle.t[2]].t[ref3] = idxA3;
    }

    bool flipped = checkAndFlip(idxA1, 0, adj);
    flipped = checkAndFlip(idxA2, 0, adj);
    flipped = checkAndFlip(idxA3, 0, adj);
}

void removeVirtualPoints(vector<Point> virtualPoints, TriangleAdjacencyTable& adj)
{
    for(auto it = adj.table.begin(); it != adj.table.end();)
    {
        bool shouldRemove = false;
        for(auto& p : it->p)
            for(auto& vp : virtualPoints)
                if (p == vp)
                {
                    shouldRemove = true;
                    break;
                }
        if (shouldRemove)
            it = adj.table.erase(it);
        else it++;
    }
}

void delunayTriangulate(vector<Point>& points, TriangleAdjacencyTable& adj)
{
    vector<Point> virtualPoints = buildWrappingTriangle(points, adj);
    vector<Point> slicedPoints = {points.begin(), points.end()};
    // insert one point at a time
    for(auto& p : slicedPoints)
    {
        int t = adj.findContainingTriangle(p);
        if (t < 0)
        {
            cout << "!! Error !! Could not find triangle containing point " << p.toStr() << "\n";
            exit(1);
        }
        divideTriangle(t, p, adj);
    }
    removeVirtualPoints(virtualPoints, adj);
}