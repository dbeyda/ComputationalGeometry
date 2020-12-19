#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <algorithm>

#include "canvas.h"
#include "polygonAlgorithms.h"
#include "fileHandler.h"
#include "triangleAdjacencyTable.h"

const char INPUT_FILE[] = "nuvem2.txt";
const char OUTPUT_FILE[] = "delunay2.txt";

bool POINTS_LABEL = false;
bool TRIANGLES_LABEL = false;

using namespace std;

int main(int argc, char *argv[])
{

    vector<Point> points;
    TriangleAdjacencyTable adjTable;

    clock_t start, end;

    pointsFromFile(points, INPUT_FILE);
    // generateRandomPoints(points, 50000);
    start = clock();
    delunayTriangulate(points, adjTable);
    end = clock();
    delunayToFile(adjTable, OUTPUT_FILE);

    cout << "File: " << INPUT_FILE << "\n"
         << "Number of points: " << points.size() << "\n"
         << "Number of triangles: " << adjTable.table.size()
         << " (before removing supertriangle)\n"
         << "Time: " << 1000 * (end-start) / CLOCKS_PER_SEC << " ms\n";


    // The code below is just to generate the visualization. ###################################


    vector<vector<double>> trianglesVector(adjTable.table.size());
    for(int i=0; i<adjTable.table.size(); ++i)
        trianglesVector[i] = {
            adjTable.table[i].p[0].x,
            adjTable.table[i].p[0].y,
            adjTable.table[i].p[1].x,
            adjTable.table[i].p[1].y,
            adjTable.table[i].p[2].x,
            adjTable.table[i].p[2].y,
        };

    vector<vector<double>> pointsVector(points.size());
    for(int i=0; i<points.size(); ++i)
        pointsVector[i] = {points[i].x, points[i].y};

    auto app = Gtk::Application::create(argc, argv, "delunay.triangulation");
    Gtk::Window window;
    window.resize(800,600);
    window.set_title("Delunay Triangulation");

    CCanvas area;
    area.points = &pointsVector;
    area.triangles = &trianglesVector;
    area.pointsLabel = POINTS_LABEL;
    area.trianglesLabel = TRIANGLES_LABEL;
    window.add(area);
    area.show();
    int run = app->run(window);
    return run;
    // return 0;
}
