#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>
#include <vector>
#include <string>

#include "canvas.h"
#include "convexHull.h"
#include "aux.h"

const char INPUT_FILE[] = "fecho2.txt";
const char OUTPUT_FILE[] = "saida2.txt";

using namespace std;

int main(int argc, char *argv[])
{
    vector<Point> points;
    vector<Point> hull;
    vector<int> hullIds;

    pointsFromFile(points, INPUT_FILE);
    findConvexHull(points, hull, hullIds);
    //hullToFile(hullIds, OUTPUT_FILE);

    // The code below is just to generate the visualization. ###################################

    vector<vector<double>> pointsVector(points.size());
    for(int i=0; i<points.size(); ++i)
        pointsVector[i] = {points[i].x, points[i].y};

    vector<vector<double>> hullVector(hull.size());
        for(int i=0; i<hull.size(); ++i)
            hullVector[i] = {hull[i].x, hull[i].y};

    // closing hull for drawing
    hullVector.push_back(hullVector.front());

    auto app = Gtk::Application::create(argc, argv, "triangulated.polygons");

    Gtk::Window window;
    window.resize(800,600);
    window.set_title("Polygon Triangulation");

    CCanvas area;
    area.points = &pointsVector;
    area.hull = &hullVector;
    window.add(area);
    area.show();

    return app->run(window);
    // return 0;
}
