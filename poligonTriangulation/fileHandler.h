#pragma once
#include "point.h"
#include <string>
#include <vector>

void poligonFromFile(vector<Point>& points, const char* path);
void diagonalsToFile(vector<int>& ids, const char* path);

