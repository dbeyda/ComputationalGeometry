#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>
#include <vector>

#include "canvas.h"
#include "circleAlgorithms.h"
#include "pointGenerator.h"

using namespace std;

int main(int argc, char** argv)
{
    vector<Point> points;
    // pointsFromFile(points, "points.txt");
    generateRandomPoints(points, 1000, -400, 400);

    vector<vector<double>> pointsVector(points.size());
    for(int i=0; i<points.size(); ++i)
        pointsVector[i] = {points[i].x, points[i].y};


    Circle circleHeur = minCircleHeuristic(points);
    Circle circleReal = minCircle(points);

    vector<vector<double>> circles = {
        {circleHeur.center.x, circleHeur.center.y, circleHeur.radius, 0.2, 0.8, 0.2},
        {circleReal.center.x, circleReal.center.y, circleReal.radius, 0.8, 0.2, 0.3},
    };


    // Application code: draw points and minimum circles
    auto app = Gtk::Application::create(argc, argv, "min.circles");

    Gtk::Window window;
    window.resize(800,600);
    window.set_title("Minimal Circle Algorithms");

    CCanvas area;
    area.points = &pointsVector;
    area.circles = &circles;
    window.add(area);
    area.show();

    return app->run(window);
}