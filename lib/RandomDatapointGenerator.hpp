/**
 * @brief Construct a new Random Data Point Generator object
 * 
 */
class RandomDataPointGenerator {
public:
    /**
     * @brief Construct a new Random Data Point Generator object
     * 
     * @param slope Slope of the line
     * @param intercept Constant value of the line
     * @param distance Variance of noise
     */
    RandomDataPointGenerator(double slope, double intercept, double distance)
        : slope_(slope),
          intercept_(intercept),
          distance_(distance) {}

    /**
     * @brief Random data generator
     * 
     * @param numPoints Number of points to be generated
     * @return std::vector<double> 
     */
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