#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "matplotlibcpp.h"
#include "ransac.hpp"
#include "RandomDatapointGenerator.hpp"

namespace plt = matplotlibcpp;

LinearModel solver(std::vector<double> x, std::vector<double> y, double threshold, int maxIterations, int sampleSize, int minInliers){
    RANSAC ransac(threshold, maxIterations, sampleSize, minInliers);
    return ransac.fit(x,y);
}

/**
 * @brief Visualize the data and the fitted model and export to a plot image
 * 
 * @param x 
 * @param y 
 * @param model 
 */
void visualize(std::vector<double> x, std::vector<double> y, LinearModel model){
    plt::title("Ransac model visualization");
    plt::plot(x, y, "bo");  // Data points
    std::vector<double> fittedY(x.size());
    for (int i = 0; i < x.size(); ++i) {
        fittedY[i] = model.a * x[i] + model.b;
    }
    plt::plot(x, fittedY, "r-");  // Fitted line
    plt::save("../img/plot.png");
}

int main() {

    // Set line parameters

    double slope = 3; 
    double intercept = 10;
    double distance = 50;
    int dataPoints = 100;

    // Set ransac parameters

    double threshold = 5.0;
    int maxIterations = 1000;
    int sampleSize = 2;
    int minInliers = 80;

    RandomDataPointGenerator dataGenerator(slope, intercept, distance);
    std::vector<double> x(dataPoints) ;
    std::iota (std::begin(x), std::end(x), 0);
    std::vector<double> y = dataGenerator.generateData(dataPoints);

    LinearModel model = solver(x, y, threshold, maxIterations, sampleSize, dataPoints);

    visualize(x, y, model);

    return 0;
}
