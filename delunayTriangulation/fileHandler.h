#pragma once
#include "point.h"
#include <string>
#include <vector>
#include "triangleAdjacencyTable.h"

void pointsFromFile(vector<Point>& points, const char* path);
void delunayToFile(TriangleAdjacencyTable& adj, const char* path);
void generateRandomPoints(vector<Point>& points, int n);