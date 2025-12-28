#include "math/statistics.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <map>

namespace temp2::math {

// =============================================================================
// Statistics
// =============================================================================

double Statistics::mean(const std::vector<double>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Empty data set");
    }
    return sum(data) / data.size();
}

double Statistics::median(std::vector<double> data) {
    if (data.empty()) {
        throw std::invalid_argument("Empty data set");
    }
    std::sort(data.begin(), data.end());
    size_t n = data.size();
    if (n % 2 == 0) {
        return (data[n/2 - 1] + data[n/2]) / 2.0;
    }
    return data[n/2];
}

double Statistics::mode(const std::vector<double>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Empty data set");
    }

    std::map<double, int> freq;
    for (double val : data) {
        freq[val]++;
    }

    double mode_val = data[0];
    int max_count = 0;
    for (const auto& [val, count] : freq) {
        if (count > max_count) {
            max_count = count;
            mode_val = val;
        }
    }
    return mode_val;
}

double Statistics::geometric_mean(const std::vector<double>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Empty data set");
    }
    double log_sum = 0;
    for (double val : data) {
        if (val <= 0) {
            throw std::invalid_argument("Geometric mean requires positive values");
        }
        log_sum += std::log(val);
    }
    return std::exp(log_sum / data.size());
}

double Statistics::harmonic_mean(const std::vector<double>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Empty data set");
    }
    double reciprocal_sum = 0;
    for (double val : data) {
        if (val == 0) {
            throw std::invalid_argument("Harmonic mean cannot include zero");
        }
        reciprocal_sum += 1.0 / val;
    }
    return data.size() / reciprocal_sum;
}

double Statistics::variance(const std::vector<double>& data) {
    if (data.size() < 2) {
        throw std::invalid_argument("Variance requires at least 2 data points");
    }
    double m = mean(data);
    double sum_sq = 0;
    for (double val : data) {
        double diff = val - m;
        sum_sq += diff * diff;
    }
    return sum_sq / (data.size() - 1);  // Sample variance
}

double Statistics::standard_deviation(const std::vector<double>& data) {
    return std::sqrt(variance(data));
}

double Statistics::range(const std::vector<double>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Empty data set");
    }
    auto [min_it, max_it] = std::minmax_element(data.begin(), data.end());
    return *max_it - *min_it;
}

double Statistics::interquartile_range(std::vector<double> data) {
    return quartile_3(data) - quartile_1(data);
}

double Statistics::percentile(std::vector<double> data, double p) {
    if (data.empty()) {
        throw std::invalid_argument("Empty data set");
    }
    if (p < 0 || p > 100) {
        throw std::invalid_argument("Percentile must be between 0 and 100");
    }

    std::sort(data.begin(), data.end());
    double index = (p / 100.0) * (data.size() - 1);
    size_t lower = static_cast<size_t>(std::floor(index));
    size_t upper = static_cast<size_t>(std::ceil(index));

    if (lower == upper) {
        return data[lower];
    }

    double weight = index - lower;
    return data[lower] * (1 - weight) + data[upper] * weight;
}

double Statistics::quartile_1(std::vector<double> data) {
    return percentile(std::move(data), 25);
}

double Statistics::quartile_3(std::vector<double> data) {
    return percentile(std::move(data), 75);
}

double Statistics::skewness(const std::vector<double>& data) {
    if (data.size() < 3) {
        throw std::invalid_argument("Skewness requires at least 3 data points");
    }
    double m = mean(data);
    double sd = standard_deviation(data);
    if (sd == 0) return 0;

    double sum_cubed = 0;
    for (double val : data) {
        double z = (val - m) / sd;
        sum_cubed += z * z * z;
    }

    double n = data.size();
    return (n / ((n - 1) * (n - 2))) * sum_cubed;
}

double Statistics::kurtosis(const std::vector<double>& data) {
    if (data.size() < 4) {
        throw std::invalid_argument("Kurtosis requires at least 4 data points");
    }
    double m = mean(data);
    double sd = standard_deviation(data);
    if (sd == 0) return 0;

    double sum_fourth = 0;
    for (double val : data) {
        double z = (val - m) / sd;
        sum_fourth += z * z * z * z;
    }

    double n = data.size();
    double excess = ((n * (n + 1)) / ((n - 1) * (n - 2) * (n - 3))) * sum_fourth;
    excess -= (3 * (n - 1) * (n - 1)) / ((n - 2) * (n - 3));
    return excess;
}

double Statistics::covariance(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Data sets must have same size");
    }
    if (x.size() < 2) {
        throw std::invalid_argument("Covariance requires at least 2 data points");
    }

    double mean_x = mean(x);
    double mean_y = mean(y);
    double sum_prod = 0;

    for (size_t i = 0; i < x.size(); ++i) {
        sum_prod += (x[i] - mean_x) * (y[i] - mean_y);
    }

    return sum_prod / (x.size() - 1);
}

double Statistics::correlation(const std::vector<double>& x, const std::vector<double>& y) {
    double cov = covariance(x, y);
    double sd_x = standard_deviation(x);
    double sd_y = standard_deviation(y);

    if (sd_x == 0 || sd_y == 0) return 0;
    return cov / (sd_x * sd_y);
}

double Statistics::sum(const std::vector<double>& data) {
    double total = 0;
    for (double val : data) {
        total += val;
    }
    return total;
}

double Statistics::product(const std::vector<double>& data) {
    if (data.empty()) return 0;
    double total = 1;
    for (double val : data) {
        total *= val;
    }
    return total;
}

double Statistics::min(const std::vector<double>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Empty data set");
    }
    return *std::min_element(data.begin(), data.end());
}

double Statistics::max(const std::vector<double>& data) {
    if (data.empty()) {
        throw std::invalid_argument("Empty data set");
    }
    return *std::max_element(data.begin(), data.end());
}

size_t Statistics::count(const std::vector<double>& data) {
    return data.size();
}

// =============================================================================
// LinearRegression
// =============================================================================

LinearRegression::LinearRegression() : slope(0), intercept(0), r_squared(0) {}

void LinearRegression::fit(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size() || x.size() < 2) {
        throw std::invalid_argument("Invalid data for regression");
    }

    double n = x.size();
    double sum_x = Statistics::sum(x);
    double sum_y = Statistics::sum(y);
    double mean_x = sum_x / n;
    double mean_y = sum_y / n;

    double ss_xx = 0, ss_xy = 0, ss_yy = 0;
    for (size_t i = 0; i < x.size(); ++i) {
        double dx = x[i] - mean_x;
        double dy = y[i] - mean_y;
        ss_xx += dx * dx;
        ss_xy += dx * dy;
        ss_yy += dy * dy;
    }

    if (ss_xx == 0) {
        throw std::invalid_argument("Cannot fit vertical line");
    }

    slope = ss_xy / ss_xx;
    intercept = mean_y - slope * mean_x;
    r_squared = (ss_yy > 0) ? (ss_xy * ss_xy) / (ss_xx * ss_yy) : 1.0;
}

double LinearRegression::predict(double x) const {
    return slope * x + intercept;
}

std::vector<double> LinearRegression::predict(const std::vector<double>& x) const {
    std::vector<double> result;
    result.reserve(x.size());
    for (double val : x) {
        result.push_back(predict(val));
    }
    return result;
}

double LinearRegression::residual(double x, double y) const {
    return y - predict(x);
}

std::vector<double> LinearRegression::residuals(const std::vector<double>& x,
                                                  const std::vector<double>& y) const {
    std::vector<double> result;
    result.reserve(x.size());
    for (size_t i = 0; i < x.size(); ++i) {
        result.push_back(residual(x[i], y[i]));
    }
    return result;
}

// =============================================================================
// Histogram
// =============================================================================

Histogram::Histogram(int num_bins, double min_val, double max_val)
    : bins(num_bins, 0), min_value(min_val), max_value(max_val) {
    if (num_bins <= 0) {
        throw std::invalid_argument("Number of bins must be positive");
    }
    if (min_val >= max_val) {
        throw std::invalid_argument("min_value must be less than max_value");
    }
    bin_width = (max_value - min_value) / num_bins;
}

void Histogram::add(double value) {
    int bin = get_bin(value);
    if (bin >= 0 && bin < static_cast<int>(bins.size())) {
        bins[bin]++;
    }
}

void Histogram::add_range(const std::vector<double>& values) {
    for (double val : values) {
        add(val);
    }
}

int Histogram::get_bin(double value) const {
    if (value < min_value || value > max_value) {
        return -1;
    }
    int bin = static_cast<int>((value - min_value) / bin_width);
    if (bin >= static_cast<int>(bins.size())) {
        bin = bins.size() - 1;
    }
    return bin;
}

void Histogram::clear() {
    std::fill(bins.begin(), bins.end(), 0);
}

double Histogram::bin_center(int bin_index) const {
    return min_value + bin_width * (bin_index + 0.5);
}

int Histogram::total_count() const {
    int total = 0;
    for (int count : bins) {
        total += count;
    }
    return total;
}

double Histogram::frequency(int bin_index) const {
    int total = total_count();
    if (total == 0) return 0;
    return static_cast<double>(bins[bin_index]) / total;
}

// =============================================================================
// MovingAverage
// =============================================================================

std::vector<double> MovingAverage::sma(const std::vector<double>& data, int window) {
    if (window <= 0 || static_cast<size_t>(window) > data.size()) {
        throw std::invalid_argument("Invalid window size");
    }

    std::vector<double> result;
    result.reserve(data.size() - window + 1);

    double sum = 0;
    for (int i = 0; i < window; ++i) {
        sum += data[i];
    }
    result.push_back(sum / window);

    for (size_t i = window; i < data.size(); ++i) {
        sum += data[i] - data[i - window];
        result.push_back(sum / window);
    }

    return result;
}

std::vector<double> MovingAverage::ema(const std::vector<double>& data, double alpha) {
    if (alpha <= 0 || alpha > 1) {
        throw std::invalid_argument("Alpha must be in (0, 1]");
    }
    if (data.empty()) return {};

    std::vector<double> result;
    result.reserve(data.size());

    result.push_back(data[0]);
    for (size_t i = 1; i < data.size(); ++i) {
        double ema_val = alpha * data[i] + (1 - alpha) * result.back();
        result.push_back(ema_val);
    }

    return result;
}

std::vector<double> MovingAverage::wma(const std::vector<double>& data,
                                        const std::vector<double>& weights) {
    if (weights.empty() || weights.size() > data.size()) {
        throw std::invalid_argument("Invalid weights");
    }

    double weight_sum = 0;
    for (double w : weights) {
        weight_sum += w;
    }
    if (weight_sum == 0) {
        throw std::invalid_argument("Weights sum to zero");
    }

    std::vector<double> result;
    result.reserve(data.size() - weights.size() + 1);

    for (size_t i = 0; i <= data.size() - weights.size(); ++i) {
        double weighted_sum = 0;
        for (size_t j = 0; j < weights.size(); ++j) {
            weighted_sum += data[i + j] * weights[j];
        }
        result.push_back(weighted_sum / weight_sum);
    }

    return result;
}

std::vector<double> MovingAverage::cma(const std::vector<double>& data) {
    if (data.empty()) return {};

    std::vector<double> result;
    result.reserve(data.size());

    double sum = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i];
        result.push_back(sum / (i + 1));
    }

    return result;
}

// =============================================================================
// RandomGenerator
// =============================================================================

RandomGenerator::RandomGenerator() : seed_(12345), current_(seed_) {}

RandomGenerator::RandomGenerator(unsigned int seed) : seed_(seed), current_(seed) {}

unsigned int RandomGenerator::next() {
    // Linear congruential generator
    current_ = current_ * 1103515245 + 12345;
    return (current_ >> 16) & 0x7FFF;
}

int RandomGenerator::uniform_int(int min, int max) {
    if (min > max) std::swap(min, max);
    return min + static_cast<int>(next() % (max - min + 1));
}

double RandomGenerator::uniform_real(double min, double max) {
    if (min > max) std::swap(min, max);
    return min + (static_cast<double>(next()) / 0x7FFF) * (max - min);
}

double RandomGenerator::normal(double mean, double stddev) {
    // Box-Muller transform
    double u1 = uniform_real(0.0001, 1.0);
    double u2 = uniform_real(0.0001, 1.0);
    double z0 = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2);
    return mean + z0 * stddev;
}

double RandomGenerator::exponential(double lambda) {
    if (lambda <= 0) {
        throw std::invalid_argument("Lambda must be positive");
    }
    return -std::log(1.0 - uniform_real(0, 1)) / lambda;
}

int RandomGenerator::poisson(double lambda) {
    if (lambda <= 0) {
        throw std::invalid_argument("Lambda must be positive");
    }
    double L = std::exp(-lambda);
    int k = 0;
    double p = 1.0;
    do {
        k++;
        p *= uniform_real(0, 1);
    } while (p > L);
    return k - 1;
}

bool RandomGenerator::bernoulli(double p) {
    return uniform_real(0, 1) < p;
}

void RandomGenerator::set_seed(unsigned int seed) {
    seed_ = seed;
    current_ = seed;
}

std::vector<double> RandomGenerator::generate_sequence(int count, double min, double max) {
    std::vector<double> result;
    result.reserve(count);
    for (int i = 0; i < count; ++i) {
        result.push_back(uniform_real(min, max));
    }
    return result;
}

void RandomGenerator::shuffle(std::vector<int>& data) {
    for (size_t i = data.size() - 1; i > 0; --i) {
        size_t j = next() % (i + 1);
        std::swap(data[i], data[j]);
    }
}

}  // namespace temp2::math
