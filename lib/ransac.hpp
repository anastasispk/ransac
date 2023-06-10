// Define a linear model as y = mx + c
struct LinearModel {
    double m;  // Slope
    double c;  // Intercept
};

class RANSAC {
public:
    RANSAC(double threshold, int maxIterations, int sampleSize, int minInliers)
        : threshold_(threshold),
          maxIterations_(maxIterations),
          sampleSize_(sampleSize),
          minInliers_(minInliers) {}

    LinearModel fit(const std::vector<double>& x, const std::vector<double>& y) {
        if (x.size() != y.size() || x.size() < sampleSize_ || x.size() < minInliers_) {
            std::cerr << "Invalid input data!" << std::endl;
            return LinearModel();
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        int bestInlierCount = 0;
        LinearModel bestModel;

        for (int i = 0; i < maxIterations_; ++i) {
            // Randomly sample points
            std::vector<int> indices = randomSample(x.size(), sampleSize_, gen);

            // Fit a model to the sampled points
            LinearModel model = fitModel(x, y, indices);

            // Count inliers
            int inlierCount = countInliers(x, y, model);

            // Check if it's the best model so far
            if (inlierCount > bestInlierCount) {
                bestInlierCount = inlierCount;
                bestModel = model;

                // Early termination if we have enough inliers
                if (inlierCount >= minInliers_)
                    break;
            }
        }

        std::cout << "Best model: m = " << bestModel.m << ", c = " << bestModel.c << std::endl;
        std::cout << "Inlier count: " << bestInlierCount << std::endl;

        return bestModel;
    }

private:
    double threshold_;      // Distance threshold for inliers
    int maxIterations_;     // Maximum number of RANSAC iterations
    int sampleSize_;        // Number of points to sample for fitting the model
    int minInliers_;        // Minimum number of inliers required for a successful fit

    std::vector<int> randomSample(int dataSize, int sampleSize, std::mt19937& gen) {
        std::vector<int> indices(dataSize);
        std::iota(indices.begin(), indices.end(), 0);

        std::shuffle(indices.begin(), indices.end(), gen);

        return std::vector<int>(indices.begin(), indices.begin() + sampleSize);
    }

    LinearModel fitModel(const std::vector<double>& x, const std::vector<double>& y, const std::vector<int>& indices) {
        double sumX = 0.0, sumY = 0.0;
        double sumXSquared = 0.0, sumXY = 0.0;
        int n = indices.size();

        for (int i : indices) {
            sumX += x[i];
            sumY += y[i];
            sumXSquared += x[i] * x[i];
            sumXY += x[i] * y[i];
        }

        double denominator = n * sumXSquared - sumX * sumX;

        if (std::abs(denominator) < 1e-6) {
            return LinearModel();
        }

        double m = (n * sumXY - sumX * sumY) / denominator;
        double c = (sumY - m * sumX) / n;

        return { m, c };
    }

    int countInliers(const std::vector<double>& x, const std::vector<double>& y, const LinearModel& model) {
        int inlierCount = 0;

        for (int i = 0; i < x.size(); ++i) {
            double predictedY = model.m * x[i] + model.c;
            double error = std::abs(predictedY - y[i]);

            if (error < threshold_) {
                inlierCount++;
            }
        }

        return inlierCount;
    }
};