#ifndef SMARTCAMPUS_TEST_HARNESS_HPP
#define SMARTCAMPUS_TEST_HARNESS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <functional>

namespace smartcampus::test {

class TestRunner {
private:
    int m_totalTests{0};
    int m_passedTests{0};
    int m_failedTests{0};
    std::string m_currentSuite;

public:
    static TestRunner& instance() {
        static TestRunner s_runner;
        return s_runner;
    }

    void startSuite(const std::string& suiteName) {
        m_currentSuite = suiteName;
        std::cout << "\n[TEST SUITE] " << suiteName << "\n";
    }

    void recordResult(const std::string& testName, bool passed, const std::string& detail = "") {
        ++m_totalTests;
        if (passed) {
            ++m_passedTests;
            std::cout << "  [PASS] " << testName << "\n";
        } else {
            ++m_failedTests;
            std::cout << "  [FAIL] " << testName << " -> " << detail << "\n";
        }
    }

    int report() const {
        std::cout << "\n=======================================================\n";
        std::cout << " TEST EXECUTION SUMMARY\n";
        std::cout << " Total Assertions/Tests: " << m_totalTests << "\n";
        std::cout << " Passed:                " << m_passedTests << "\n";
        std::cout << " Failed:                " << m_failedTests << "\n";
        std::cout << " Result:                " << (m_failedTests == 0 ? "ALL TESTS PASSED! [OK]" : "SOME TESTS FAILED! [ERROR]") << "\n";
        std::cout << "=======================================================\n";
        return (m_failedTests == 0) ? 0 : 1;
    }
};

} // namespace smartcampus::test

#define TEST_SUITE(name) \
    smartcampus::test::TestRunner::instance().startSuite(name)

#define ASSERT_TRUE(expr, msg) \
    smartcampus::test::TestRunner::instance().recordResult(msg, (expr), "Expected true, got false")

#define ASSERT_FALSE(expr, msg) \
    smartcampus::test::TestRunner::instance().recordResult(msg, !(expr), "Expected false, got true")

#define ASSERT_NEAR(a, b, eps, msg) \
    smartcampus::test::TestRunner::instance().recordResult(msg, (std::fabs((a) - (b)) <= (eps)), \
        "Diff: " + std::to_string(std::fabs((a) - (b))) + " (expected " + std::to_string(b) + ", got " + std::to_string(a) + ")")

#define ASSERT_EQ(a, b, msg) \
    smartcampus::test::TestRunner::instance().recordResult(msg, ((a) == (b)), "Values not equal")

#define ASSERT_THROWS(expr, ExceptionType, msg) \
    do { \
        bool caught = false; \
        try { \
            expr; \
        } catch (const ExceptionType&) { \
            caught = true; \
        } catch (...) {} \
        smartcampus::test::TestRunner::instance().recordResult(msg, caught, "Expected exception not thrown"); \
    } while (0)

#endif // SMARTCAMPUS_TEST_HARNESS_HPP
