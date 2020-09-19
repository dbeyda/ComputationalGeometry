#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>
#include <vector>
#include <string>

#include "canvas.h"
#include "convexHull.h"
#include "aux.h"

const char INPUT_FILE[] = "fecho1.txt";
const char OUTPUT_FILE[] = "saida1.txt";

using namespace std;

int main(int argc, char *argv[])
{
    vector<Point> points;
    vector<int> hullIds;

    // pointsFromFile(points, INPUT_FILE);
    generateRandomPoints(points, 1000, 400, 400);
    findConvexHull(points, hullIds);
    hullToFile(hullIds, OUTPUT_FILE);

    // The code below is just to generate the visualization. ###################################

    vector<vector<double>> pointsVector(points.size());
    for(int i=0; i<points.size(); ++i)
        pointsVector[i] = {points[i].x, points[i].y};

    vector<vector<double>> hullVector(hullIds.size());
    for(int i=0; i<hullIds.size(); ++i)
    {
        auto& hullPoint = points[hullIds[i]];
        hullVector[i] = {hullPoint.x, hullPoint.y};
    }

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
