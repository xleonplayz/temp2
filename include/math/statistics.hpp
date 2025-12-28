#ifndef TEMP2_MATH_STATISTICS_HPP
#define TEMP2_MATH_STATISTICS_HPP

#include <vector>
#include <utility>

namespace temp2::math {

/**
 * @brief Descriptive statistics
 */
class Statistics {
public:
    // Central tendency
    static double mean(const std::vector<double>& data);
    static double median(std::vector<double> data);  // Takes copy for sorting
    static double mode(const std::vector<double>& data);
    static double geometric_mean(const std::vector<double>& data);
    static double harmonic_mean(const std::vector<double>& data);

    // Dispersion
    static double variance(const std::vector<double>& data);
    static double standard_deviation(const std::vector<double>& data);
    static double range(const std::vector<double>& data);
    static double interquartile_range(std::vector<double> data);

    // Quartiles
    static double percentile(std::vector<double> data, double p);
    static double quartile_1(std::vector<double> data);
    static double quartile_3(std::vector<double> data);

    // Other measures
    static double skewness(const std::vector<double>& data);
    static double kurtosis(const std::vector<double>& data);
    static double covariance(const std::vector<double>& x, const std::vector<double>& y);
    static double correlation(const std::vector<double>& x, const std::vector<double>& y);

    // Aggregations
    static double sum(const std::vector<double>& data);
    static double product(const std::vector<double>& data);
    static double min(const std::vector<double>& data);
    static double max(const std::vector<double>& data);
    static size_t count(const std::vector<double>& data);
};

/**
 * @brief Linear regression
 */
class LinearRegression {
public:
    double slope;
    double intercept;
    double r_squared;

    LinearRegression();

    void fit(const std::vector<double>& x, const std::vector<double>& y);
    double predict(double x) const;
    std::vector<double> predict(const std::vector<double>& x) const;

    double residual(double x, double y) const;
    std::vector<double> residuals(const std::vector<double>& x,
                                   const std::vector<double>& y) const;
};

/**
 * @brief Histogram operations
 */
class Histogram {
public:
    std::vector<int> bins;
    double min_value;
    double max_value;
    double bin_width;

    Histogram(int num_bins, double min_val, double max_val);

    void add(double value);
    void add_range(const std::vector<double>& values);
    int get_bin(double value) const;
    void clear();

    double bin_center(int bin_index) const;
    int total_count() const;
    double frequency(int bin_index) const;
};

/**
 * @brief Moving average calculations
 */
class MovingAverage {
public:
    // Simple moving average
    static std::vector<double> sma(const std::vector<double>& data, int window);

    // Exponential moving average
    static std::vector<double> ema(const std::vector<double>& data, double alpha);

    // Weighted moving average
    static std::vector<double> wma(const std::vector<double>& data,
                                    const std::vector<double>& weights);

    // Cumulative moving average
    static std::vector<double> cma(const std::vector<double>& data);
};

/**
 * @brief Random number generation utilities
 */
class RandomGenerator {
public:
    RandomGenerator();
    explicit RandomGenerator(unsigned int seed);

    // Uniform distribution
    int uniform_int(int min, int max);
    double uniform_real(double min, double max);

    // Normal distribution
    double normal(double mean, double stddev);

    // Other distributions
    double exponential(double lambda);
    int poisson(double lambda);
    bool bernoulli(double p);

    // Utility
    void set_seed(unsigned int seed);
    std::vector<double> generate_sequence(int count, double min, double max);
    void shuffle(std::vector<int>& data);

private:
    unsigned int seed_;
    unsigned int current_;
    unsigned int next();
};

}  // namespace temp2::math

#endif  // TEMP2_MATH_STATISTICS_HPP
