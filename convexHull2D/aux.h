#pragma once
#include "point.h"
#include <string>
#include <vector>

void pointsFromFile(vector<Point>& points, const char* path);
void hullToFile(vector<int>& ids, const char* path);
void generateRandomPoints(vector<Point>& points, int n, double xc, double yc);

