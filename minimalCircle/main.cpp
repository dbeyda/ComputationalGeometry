#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>
#include <vector>

#include "canvas.h"
#include "circleAlgorithms.h"

using namespace std;

int main(int argc, char** argv)
{
    vector<vector<double>> points = {
        {0.1, 0.2},
        {-0.3, 0.2},
        {0.5, 0.7},
        {-0.2, -0.9},
        {0.77, 0.4},
        {0.4, -0.1},
        {-0.1, 0.8},
        {-0.4, 0.8},
        {0.2, 0.7},
        {0.1, 0.2},
        {-0.3, -0.2},
        {-0.5, -0.7},
        {0.8, -0.8},
        {0.8, -0.9},
    };

    vector<double> circleHeur = minCircleHeuristic(points);
    vector<double> circleReal = minCircle(points);

    vector<vector<double>> circles = {
        {circleHeur[0], circleHeur[1], circleHeur[2], 0.3, 0.0, 0.8},
        {circleReal[0], circleReal[1], circleReal[2], 0.8, 0.2, 0.3},
    };


    // Application code: draw points and minimum circles
    auto app = Gtk::Application::create(argc, argv, "min.circles");

    Gtk::Window window;
    window.resize(800,600);
    window.set_title("Minimal Circle Algorithms");

    CCanvas area;
    area.points = &points;
    area.circles = &circles;
    window.add(area);
    area.show();

    return app->run(window);
}