#include "polyImporter.h"
#include <fstream>
#include <random>
#include <iostream>
#include <cmath>

void poligonFromFile(vector<Point>& points, string path)
{
    string line;
    double x, y;
    ifstream myfile (path.c_str());
    if (myfile.is_open())
    {
        while (getline(myfile,line))
        {
            sscanf(line.c_str(), "%lf %lf", &x, &y);
            points.push_back({x, y});
        }
        myfile.close();
    }
}
