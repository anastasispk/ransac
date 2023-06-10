/**
 * @brief Define a linear model as y = ax + b
 * 
 * @param a Slope
 * @param b Intercept
 */
struct LinearModel {
    double a;
    double b;
};

/**
 * @brief Construct a new RANSAC object
 * 
 * @param threshold Distance threshold for inliers
 * @param maxIterations Maximum number of RANSAC iterations
 * @param sampleSize Number of points to sample for fitting the model
 * @param minInliers Minimum number of inliers required for a successful fit
 */
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

        std::cout << "Best model: a = " << bestModel.a << ", b = " << bestModel.b << std::endl;
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

        double a = (n * sumXY - sumX * sumY) / denominator;
        double b = (sumY - a * sumX) / n;

        return { a, b };
    }

    /**
     * @brief Count the number of inliers during ransac
     * 
     * @param x 
     * @param y 
     * @param model 
     * @return int number of inliers
     */
    int countInliers(const std::vector<double>& x, const std::vector<double>& y, const LinearModel& model) {
        int inlierCount = 0;

        for (int i = 0; i < x.size(); ++i) {
            double predictedY = model.a * x[i] + model.b;
            double error = std::abs(predictedY - y[i]);

            if (error < threshold_) {
                inlierCount++;
            }
        }

        return inlierCount;
    }
};