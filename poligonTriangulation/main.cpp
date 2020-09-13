#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>
#include <vector>

#include "canvas.h"
#include "triangulationAlgorithm.h"
#include "polyImporter.h"

using namespace std;

int main(int argc, char** argv)
{
    vector<Point> poly;

    // poligonFromFile(poly, "polygon1.txt");
    poligonFromFile(poly, "polygon2.txt");


    // The code below is just to generate the visualization.
    // It can be commented while running the tests with random numbers

    // Application code: draw points and minimum circles

    vector<vector<double>> pointsVector(poly.size());
    for(int i=0; i<poly.size(); ++i)
        pointsVector[i] = {poly[i].x, poly[i].y};

    auto app = Gtk::Application::create(argc, argv, "triangulated.polygons");

    Gtk::Window window;
    window.resize(800,600);
    window.set_title("Polygon Triangulation");

    CCanvas area;
    area.points = &pointsVector;
    window.add(area);
    area.show();

    return app->run(window);
    return 0;
}