#include "pointGenerator.h"
#include <fstream>
#include <random>
#include <iostream>
#include <cmath>

void pointsFromFile(vector<Point>& points, string path)
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

void generateRandomPoints(vector<Point>& points, int n, int lowerLimit, int upperLimit)
{
    // std::random_device rd;
    std::mt19937 gen(0);
    std::normal_distribution<double> radius(0, 80000);
    std::uniform_real_distribution<double> theta(0, 2 * M_PI);

    double r, t;
    while(n--)
    {
        r = radius(gen);
        r = (r < 0) ? -sqrt(-r) : sqrt(r);
        t = theta(gen);
        points.push_back({r * cos(t), r * sin(t)});
        // std::cout << n << " - " << points.back().x << " " << points.back().y << '\n';
    }
}