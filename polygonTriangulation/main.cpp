#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>
#include <vector>
#include <string>

#include "canvas.h"
#include "polygonAlgorithms.h"
#include "fileHandler.h"

const char INPUT_FILE[] = "polygon2.txt";
const char OUTPUT_FILE[] = "diagonals2.txt";

using namespace std;

int main(int argc, char *argv[])
{
    vector<Point> poly;
    vector<Point> diagonals;
    vector<int> diagonalIds;

    poligonFromFile(poly, INPUT_FILE);
    triangulate(poly, diagonals, diagonalIds);
    diagonalsToFile(diagonalIds, OUTPUT_FILE);

    // The code below is just to generate the visualization. ###################################

    vector<vector<double>> pointsVector(poly.size());
    for(int i=0; i<poly.size(); ++i)
        pointsVector[i] = {poly[i].x, poly[i].y};

    // closing polygon for drawing
    pointsVector.push_back(vector<double>{poly.front().x, poly.front().y});

    vector<vector<double>> diagonalsVector(diagonals.size());
    for(int i=0; i<diagonals.size(); ++i)
        diagonalsVector[i] = {diagonals[i].x, diagonals[i].y};

    auto app = Gtk::Application::create(argc, argv, "triangulated.polygons");

    Gtk::Window window;
    window.resize(800,600);
    window.set_title("Polygon Triangulation");

    CCanvas area;
    area.points = &pointsVector;
    area.diagonals = &diagonalsVector;
    window.add(area);
    area.show();

    return app->run(window);
    // return 0;
}
