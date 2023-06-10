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

void visualize(std::vector<double> x, std::vector<double> y, LinearModel model){
    // Visualize the data and the fitted model
    plt::plot(x, y, "ko");  // Data points
    std::vector<double> fittedY(x.size());
    for (int i = 0; i < x.size(); ++i) {
        fittedY[i] = model.m * x[i] + model.c;
    }
    plt::plot(x, fittedY, "r-");  // Fitted line
    plt::save("../img/plot.png");
}

int main() {

    double slope = 3; 
    double intercept = 10;
    double distance = 50;
    int dataPoints = 100;

    double threshold = 5.0;
    int maxIterations = 1000;
    int sampleSize = 2;
    int minInliers = 80;

    RandomDataPointGenerator dataGenerator(slope, intercept, distance);
    std::vector<double> x(dataPoints) ;
    std::iota (std::begin(x), std::end(x), 0);
    std::vector<double> y = dataGenerator.generateData(dataPoints);

    std::cout << x.size() << " " << y.size() << std::endl;

    LinearModel model = solver(x, y, threshold, maxIterations, sampleSize, dataPoints);

    visualize(x, y, model);

    return 0;
}
