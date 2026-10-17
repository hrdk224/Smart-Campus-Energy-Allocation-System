#ifndef SMARTCAMPUS_GENERIC_UTILS_HPP
#define SMARTCAMPUS_GENERIC_UTILS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cmath>

namespace smartcampus::utils {

/**
 * @brief Generic function template to clamp a value within [minVal, maxVal].
 * Demonstrates C++ function templates.
 */
template <typename T>
T clampValue(T val, T minVal, T maxVal) {
    if (minVal > maxVal) {
        throw std::invalid_argument("clampValue: minVal cannot be greater than maxVal");
    }
    return (val < minVal) ? minVal : ((val > maxVal) ? maxVal : val);
}

/**
 * @brief Generic class template for accumulating statistical metrics over time.
 * Demonstrates C++ class templates, member functions, and template instantation.
 */
template <typename T>
class MetricAggregator {
private:
    std::vector<T> m_samples;

public:
    MetricAggregator() = default;

    void addSample(T sample) {
        m_samples.push_back(sample);
    }

    size_t count() const {
        return m_samples.size();
    }

    bool empty() const {
        return m_samples.empty();
    }

    T sum() const {
        if (m_samples.empty()) return T{0};
        T total = T{0};
        for (const auto& s : m_samples) {
            total += s;
        }
        return total;
    }

    double average() const {
        if (m_samples.empty()) return 0.0;
        return static_cast<double>(sum()) / static_cast<double>(m_samples.size());
    }

    T min() const {
        if (m_samples.empty()) {
            throw std::runtime_error("MetricAggregator: cannot compute min of empty dataset");
        }
        return *std::min_element(m_samples.begin(), m_samples.end());
    }

    T max() const {
        if (m_samples.empty()) {
            throw std::runtime_error("MetricAggregator: cannot compute max of empty dataset");
        }
        return *std::max_element(m_samples.begin(), m_samples.end());
    }

    void clear() {
        m_samples.clear();
    }
};

/**
 * @brief Utility function to trim leading and trailing whitespace from a string.
 */
inline std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Split string by a delimiter character.
 */
inline std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

} // namespace smartcampus::utils

#endif // SMARTCAMPUS_GENERIC_UTILS_HPP
