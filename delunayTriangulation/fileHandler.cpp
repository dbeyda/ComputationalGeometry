#include "fileHandler.h"
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
        int i = 0;
        while (getline(myfile,line))
        {
            sscanf(line.c_str(), "%lf %lf", &x, &y);
            points.push_back({x, y, i});
            ++i;
        }
        myfile.close();
    }
    else
        cout << "!! Error !! Could not open input file.\n";
}

void delunayToFile(TriangleAdjacencyTable& adj, const char* path)
{
    ofstream myfile(path, ios::out | ios::trunc);
    if (myfile.is_open())
    {
       for(auto& t : adj.table)
       {
           myfile << t.p[0].id << " "
                  << t.p[1].id << " "
                  << t.p[2].id << "\n";
       }
        myfile.close();
    }
    else
        cout << "!! Error !! Could not open output file.\n";
}

void generateRandomPoints(vector<Point>& points, int n)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> randint(300, 1000);

    double x, y;
    while(n--)
    {
        x = (double) randint(gen);
        y = (double) randint(gen);
        points.push_back({x, y});
    }
}