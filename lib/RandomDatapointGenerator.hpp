class RandomDataPointGenerator {
public:
    RandomDataPointGenerator(double slope, double intercept, double distance)
        : slope_(slope),
          intercept_(intercept),
          distance_(distance) {}

    std::vector<double> generateData(int numPoints) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> distribution(-distance_, distance_);

        std::vector<double> dataPoints;

        for (int i = 0; i < numPoints; ++i) {
            double x = i;
            double y = slope_ * x + intercept_ + distribution(gen);
            dataPoints.push_back(y);
        }

        return dataPoints;
    }

private:
    double slope_;
    double intercept_;
    double distance_;
};