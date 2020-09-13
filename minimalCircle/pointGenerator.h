#pragma once
#include "point.h"
#include <string>
#include <vector>

void pointsFromFile(vector<Point>& points, string path);
void generateRandomPoints(vector<Point>&, int n, int lowerLimit, int upperLimit);
