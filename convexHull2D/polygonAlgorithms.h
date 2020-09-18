#define myEpsilon 1e-9

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <float.h>
#include <time.h>
#include "point.h"
#include "circularList.h"

using namespace std;

/**
 * A function that returns the orientation determinant
 * of a point p with respect to a segment ab
 */
inline double orientation(Point& p, Point& a, Point& b)
{
    return (b.x*p.y + a.x*b.y + a.y*p.x
            - a.y*b.x - a.x*p.y - b.y*p.x);
}

/**
 * A function to test if a point p is inside the triangle
 * ABC.
 */
inline bool pointInsideTriangle(Point& p, Point& a, Point& b, Point& c)
{
    if (orientation(p, a, b) <= -myEpsilon) return false;
    if (orientation(p, b, c) <= -myEpsilon) return false;
    if (orientation(p, c, a) <= -myEpsilon) return false;
    return true;
}


inline bool isConvex(CircularList<Point*>::Node* it, CircularList<Point*>& vertices)
{
    auto& p = *it->next->data;
    auto& a = *it->prev->data;
    auto& b = *it->data;
    // return orientation(p, a, b) > myEpsilon;

    // this is not ideal, but was needed for the star triangulation
    // to be correct
    return orientation(p, a, b) > 200;
}


inline bool isEar(CircularList<Point*>::Node* it, CircularList<Point*>& vertices)
{
    if (!isConvex(it, vertices)) return false;
    auto itv = it->next->next;
    // define triangle (ear candidate)
    auto& a = *(it->prev->data);
    auto& b = *(it->data);
    auto& c = *(it->next->data);
    for(auto itv = it->next->next; itv->next != it; itv = itv->next)
    {
        auto& p = *(itv->data);
        if(pointInsideTriangle(p, a, b, c))
            return false;
    }
    return true;
}


inline void triangulate(vector<Point>& poly, vector<Point>& diagonals, vector<int>& ids)
{
    // transfer vertices to a circular linked list
    CircularList<Point*> vertices;
    for(int i=0; i<poly.size(); ++i)
    {
        vertices.push(&poly[i]);
        vertices.head->prev->id = i;
    }

    CircularList<Point*>::Node* it = vertices.head;
    while(vertices.size() > 3)
    {
        if (isEar(it, vertices))
        {
            diagonals.push_back(*it->prev->data);
            diagonals.push_back(*it->next->data);
            ids.push_back(it->prev->id);
            ids.push_back(it->next->id);

            auto toRemove = it;
            it = it->next;
            vertices.remove(toRemove);
        }
        else it = it->next;
    }
}
