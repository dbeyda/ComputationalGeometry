#include "fileHandler.h"
#include <fstream>
#include <random>
#include <iostream>
#include <cmath>

void poligonFromFile(vector<Point>& points, const char* path)
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

void diagonalsToFile(vector<int>& ids, const char* path)
{
    ofstream myfile(path, ios::out | ios::trunc);
    if (myfile.is_open())
    {
       for(int i=0; i < ids.size(); ++i)
           myfile << ids[i] << ((i%2) ? "\n" : " ");
        myfile.close();
    }
    else
        cout << "!! Error !! Could not open output file.\n";
}
