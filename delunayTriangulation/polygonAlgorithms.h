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

typedef struct
{
    Point p1, p2, p3;
} Triangle;

/**
 * A function that returns the orientation determinant
 * of a point p with respect to a segment ab
 */
double orientation(Point& p, Point& a, Point& b)
{
    return (b.x*p.y + a.x*b.y + a.y*p.x
            - a.y*b.x - a.x*p.y - b.y*p.x);
}

/**
 * A function to test if a point p is inside the triangle
 * ABC.
 */
bool pointInsideTriangle(Point& p, Point& a, Point& b, Point& c)
{
    if (orientation(p, a, b) <= -myEpsilon) return false;
    if (orientation(p, b, c) <= -myEpsilon) return false;
    if (orientation(p, c, a) <= -myEpsilon) return false;
    return true;
}

/**
 * A function to test if a point p is inside the circle
 * formed by 3 points ABC.
 */
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
    return distance > (radius + myEpsilon);
}


bool isConvex(Point& p, Point& a, Point& b)
{
    return orientation(p, a, b) > myEpsilon;

    // this is not ideal, but was needed for the star triangulation
    // to be correct
    // return orientation(p, a, b) > 200;
}

void buildWrappingTriangle(vector<Point>& points, TriangleAdjacencyTable& adj)
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
            Point({-3*m, -3*m}) + center,
            Point({3*m, 0}) + center,
            Point({0, 3*m}) + center,
        });
}

int getContainingTriangle(Point& p, TriangleAdjacencyTable& adj)
{
    // TODO: implement this as a topological walk
    // TODO: consider case where point is at a triangle edge
    for(int i=0; i<adj.table.size(); ++i)
        if (pointInsideTriangle(p, adj.table[i].p1,adj.table[i].p2, adj.table[i].p3))
            return i;
    return -1;
}

int* getTriangleRefFromAdjTriang(int v, int t, TriangleAdjacencyTable& adj)
{
    if (t == -1) return nullptr;

    AdjacencyElement& triangle = adj.table[t];
    if(triangle.t1 == v) return &triangle.t1;
    else if(triangle.t2 == v) return &triangle.t2;
    else if(triangle.t3 == v) return &triangle.t3;
    return nullptr;
}

Point* getOppositePoint(Point* p, int t, TriangleAdjacencyTable& adj)
{
    auto& triangle = adj.table[t];
    AdjacencyElement* oppositeTriang;
    if(p == &triangle.p1) oppositeTriang = triangle.t1 >= 0 ? &adj.table[triangle.t1] : nullptr;
    else if(p == &triangle.p2) oppositeTriang = triangle.t1 >= 0 ? &adj.table[triangle.t2] : nullptr;
    else if(p == &triangle.p3) oppositeTriang =  triangle.t3 >= 0 ? &adj.table[triangle.t3] : nullptr;
    else return nullptr;
    if(!oppositeTriang) return nullptr;

    if(oppositeTriang->t3 == t) return &oppositeTriang->p3;
    if(oppositeTriang->t2 == t) return &oppositeTriang->p2;
    if(oppositeTriang->t1 == t) return &oppositeTriang->p1;
    return nullptr;
}

bool checkAndFlip(int t, Point* p, TriangleAdjacencyTable& adj)
{
    auto& triangle = adj.table[t];
    Point* oppositeP = getOppositePoint(p, t, adj);
    if (!oppositeP)
    {
        cout << "Opposite point not found!\n";
    }
    else
    {
        cout << "Opposite point " << oppositeP->toStr() << " found.\n";
    }

    // flip not needed
    if (!pointInsideCircle(*p, triangle.p1, triangle.p2, triangle.p3))
        return false;
    // if (!isConvex())

    // flip needed

    return true;
}

void divideTriangle(Point& p, int t, TriangleAdjacencyTable& adj)
{
    AdjacencyElement triangle = adj.table[t];

    // triangle will be substituted by a1
    adj.table[t] = {p, triangle.p2, triangle.p3};
    // a2 and a3 will be will elements in the list
    adj.table.push_back({p, triangle.p3, triangle.p1});
    adj.table.push_back({p, triangle.p1, triangle.p2});

    int idxA1 = t;
    int idxA2 = adj.table.size()-2;
    int idxA3 = adj.table.size()-1;

    auto& a1 = adj.table[idxA1];
    auto& a2 = adj.table[idxA2];
    auto& a3 = adj.table[idxA3];

    // setting adjacents for a1, a2, a3
    a1.t1 = triangle.t1;
    a1.t2 = idxA2;
    a1.t3 = idxA3;

    a2.t1 = triangle.t2;
    a2.t2 = idxA3;
    a2.t3 = idxA1;

    a3.t1 = triangle.t3;
    a3.t2 = idxA1;
    a3.t3 = idxA2;

    // fixing adjacent triangles
    int* ref1 = getTriangleRefFromAdjTriang(t, triangle.t1, adj);
    int* ref2 = getTriangleRefFromAdjTriang(t, triangle.t2, adj);
    int* ref3 = getTriangleRefFromAdjTriang(t, triangle.t3, adj);

    if (ref1) *ref1 = idxA1;
    if (ref2) *ref2 = idxA2;
    if (ref3) *ref3 = idxA3;

    bool flipped = checkAndFlip(idxA1, &a1.p1, adj);
    flipped ? (cout << "true\n") : (cout << "false\n");
    flipped = checkAndFlip(idxA2, &a2.p1, adj);
    flipped ? (cout << "true\n") : (cout << "false\n");
    flipped = checkAndFlip(idxA3, &a3.p1, adj);
    flipped ? (cout << "true\n") : (cout << "false\n");
}

void delunayTriangulate(vector<Point>& points, TriangleAdjacencyTable& adj)
{
    buildWrappingTriangle(points, adj);
    vector<Point> slicedPoints = {points.begin(), points.begin()+2};
    // insert one point at a time
    for(auto& p : slicedPoints)
    {
        int t = getContainingTriangle(p, adj);
        if (t < 0)
        {
            cout << "!! Error !! Could not find triangle containing point " << p.toStr() << "\n";
            exit(1);
        }
        divideTriangle(p, t, adj);


    }

}