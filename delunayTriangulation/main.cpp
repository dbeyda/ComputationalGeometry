#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>
#include <vector>
#include <string>

#include "canvas.h"
#include "polygonAlgorithms.h"
#include "fileHandler.h"
#include "triangleAdjacencyTable.h"

const char INPUT_FILE[] = "nuvem1.txt";
const char OUTPUT_FILE[] = "delunay1.txt";

using namespace std;

int main(int argc, char *argv[])
{
    vector<Point> points;
    TriangleAdjacencyTable adjTable;

    pointsFromFile(points, INPUT_FILE);
    delunayTriangulate(points, adjTable);

    // diagonalsToFile(diagonalIds, OUTPUT_FILE);

    // The code below is just to generate the visualization. ###################################

    vector<vector<double>> trianglesVector(adjTable.table.size());
    for(int i=0; i<adjTable.table.size(); ++i)
        trianglesVector[i] = {
            adjTable.table[i].p1.x,
            adjTable.table[i].p1.y,
            adjTable.table[i].p2.x,
            adjTable.table[i].p2.y,
            adjTable.table[i].p3.x,
            adjTable.table[i].p3.y,
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
    window.add(area);
    area.show();

    return app->run(window);
    // return 0;
}
