#include "aux.h"
#include <fstream>
#include <random>
#include <iostream>
#include <cmath>

void pointsFromFile(vector<Point>& points, const char* path)
{
    string line;
    double x, y;
    ifstream myfile (path);
    if (myfile.is_open())
    {
        while (getline(myfile,line))
        {
            sscanf(line.c_str(), "%lf %lf", &x, &y);
            points.push_back({x, y});
        }
        myfile.close();
    }
    else
        cout << "!! Error !! Could not open input file.\n";
}

void hullToFile(vector<int>& ids, const char* path)
{
    ofstream myfile(path, ios::out | ios::trunc);
    if (myfile.is_open())
    {
       for(int i=0; i < ids.size(); ++i)
           myfile << ids[i] << "\n";
        myfile.close();
    }
    else
        cout << "!! Error !! Could not open output file.\n";
}

void generateRandomPoints(vector<Point>& points, int n, double xc, double yc)
{
    // std::random_device rd;
    std::mt19937 gen(0);
    std::normal_distribution<double> radius(0, 12000);
    std::uniform_real_distribution<double> theta(0, 2 * M_PI);

    double r, t;
    while(n--)
    {
        r = radius(gen);
        r = (r < 0) ? -sqrt(-r) : sqrt(r);
        t = theta(gen);
        points.push_back({xc + r * cos(t), yc + r * sin(t)});
        // std::cout << n << " - " << points.back().x << " " << points.back().y << '\n';
    }
}