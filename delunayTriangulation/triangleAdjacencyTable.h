#pragma once

#include "point.h"
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

class AdjacencyElement
{
    public:
    vector<Point> p;
    vector<int> t;

    AdjacencyElement()
        : p({{0, 0}, {0, 0}, {0, 0}}),
          t({-1, -1, -1}) {};
    AdjacencyElement(Point p1, Point p2, Point p3, int t1=-1, int t2=-1, int t3=-1)
        : p({p1, p2, p3}),
          t({t1, t2, t3}) {};

    int getTriangleIdx(int i)
    {
        if (t[0] == i) return 0;
        if (t[1] == i) return 1;
        if (t[2] == i) return 2;

        throw std::logic_error("getTriangleIdx neighbour not found.");
        // cout << "!! Error !! AdjacencyElement::getTriangleIdx neighbour not found.\n";
        // exit(1);
    };

    int getPointIdx(Point point)
    {
        if (p[0] == point) return 0;
        if (p[1] == point) return 1;
        if (p[2] == point) return 2;
        return -1;
    }
};

class TriangleAdjacencyTable
{
private:
    /**
     * A function that returns the orientation determinant
     * of a point p with respect to a segment ab
     */
    static double orientation(Point& p, Point& a, Point& b);

    /**
     * A function that checks if a point p is inside a
     * triangle ABC, where the sequence of points A, B and C
     * follows the counter-clockwise orientation
     */
    static bool pointInsideTriangle(Point& p, Point& a, Point& b, Point& c);

    /**
     * A function to test if a point p is inside the circle
     * formed by 3 points ABC.
     */
    static bool pointInsideCircle(Point& p, Point& a, Point& b, Point& c);

    bool isConvex(Point& p, Point& a, Point& b);

public:
    vector<AdjacencyElement> table;
    TriangleAdjacencyTable();
    int findContainingTriangle(Point& p);
};
