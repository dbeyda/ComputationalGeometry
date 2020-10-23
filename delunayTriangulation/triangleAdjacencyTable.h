#pragma once

#include "point.h"
#include <vector>

using namespace std;

class AdjacencyElement
{
    public:
    Point p1, p2, p3;
    int t1, t2, t3;
    AdjacencyElement()
        : p1({0, 0}), p2({0, 0}), p3({0, 0}),
          t1(-1), t2(-1), t3(-1) {};
    AdjacencyElement(Point p1, Point p2, Point p3, int t1=-1, int t2=-1, int t3=-1)
        : p1(p1), p2(p2), p3(p3),
          t1(t1), t2(t2), t3(t3) {};
};

class TriangleAdjacencyTable
{
public:
    vector<AdjacencyElement> table;
    TriangleAdjacencyTable()
        : table(vector<AdjacencyElement>()) {};
};
