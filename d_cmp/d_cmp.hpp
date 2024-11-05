#include <cmath>
#include <limits>

bool d_cmp(double a, double b, double eps = 1e12 * std::numeric_limits<double>::epsilon()) {
    return std::abs(a - b) <= eps || std::abs(a - b) < (std::fmax(std::abs(a), std::abs(b)) * eps);
}