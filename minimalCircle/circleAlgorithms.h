#ifndef CIRCLE_ALGORITHMS_H
#define CIRCLE_ALGORITHMS_H

#include <vector>


using namespace std;

// returns {xCenter, yCenter, radius} of the minimum circle found
inline vector<double> minCircleHeuristic(vector<vector<double>>& points)
{
    return vector<double> ({0.1, 0.1, 0.5});
}

inline vector<double> minCircle(vector<vector<double>>& points)
{
    return vector<double> ({0.3, -0.6, 0.2});
}

#endif