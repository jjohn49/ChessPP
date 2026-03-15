//
// Minimal test framework for ChessPP unit tests.
// No external dependencies — just macros around assert + reporting.
//

#ifndef CHESSPP_TESTFRAMEWORK_H
#define CHESSPP_TESTFRAMEWORK_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>

struct TestCase {
	std::string name;
	std::function<void()> func;
};

inline std::vector<TestCase>& getTests() {
	static std::vector<TestCase> tests;
	return tests;
}

inline int registerTest(const std::string& name, std::function<void()> func) {
	getTests().push_back({name, std::move(func)});
	return 0;
}

#define TEST(name) \
	void test_##name(); \
	static int _reg_##name = registerTest(#name, test_##name); \
	void test_##name()

#define ASSERT_TRUE(expr) \
	do { \
		if (!(expr)) { \
			std::cerr << "  FAIL: " << #expr << " was false" \
			          << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
			throw std::runtime_error("assertion failed"); \
		} \
	} while(0)

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

#define ASSERT_EQ(a, b) \
	do { \
		auto _a = (a); auto _b = (b); \
		if (_a != _b) { \
			std::cerr << "  FAIL: " << #a << " == " << #b \
			          << " (" << _a << " != " << _b << ")" \
			          << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
			throw std::runtime_error("assertion failed"); \
		} \
	} while(0)

#define ASSERT_NE(a, b) \
	do { \
		auto _a = (a); auto _b = (b); \
		if (_a == _b) { \
			std::cerr << "  FAIL: " << #a << " != " << #b \
			          << " (both are " << _a << ")" \
			          << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
			throw std::runtime_error("assertion failed"); \
		} \
	} while(0)

#define ASSERT_GT(a, b) \
	do { \
		auto _a = (a); auto _b = (b); \
		if (!(_a > _b)) { \
			std::cerr << "  FAIL: " << #a << " > " << #b \
			          << " (" << _a << " <= " << _b << ")" \
			          << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
			throw std::runtime_error("assertion failed"); \
		} \
	} while(0)

#define ASSERT_LT(a, b) \
	do { \
		auto _a = (a); auto _b = (b); \
		if (!(_a < _b)) { \
			std::cerr << "  FAIL: " << #a << " < " << #b \
			          << " (" << _a << " >= " << _b << ")" \
			          << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
			throw std::runtime_error("assertion failed"); \
		} \
	} while(0)

#define ASSERT_NOT_NULL(ptr) \
	do { \
		if ((ptr) == nullptr) { \
			std::cerr << "  FAIL: " << #ptr << " was null" \
			          << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
			throw std::runtime_error("assertion failed"); \
		} \
	} while(0)

inline int runAllTests() {
	int passed{0};
	int failed{0};
	auto& tests = getTests();

	std::cout << "Running " << tests.size() << " tests...\n" << std::endl;

	for (auto& test : tests) {
		try {
			test.func();
			std::cout << "  PASS: " << test.name << std::endl;
			passed++;
		} catch (...) {
			std::cout << "  FAIL: " << test.name << std::endl;
			failed++;
		}
	}

	std::cout << "\n" << passed << " passed, " << failed << " failed, "
	          << tests.size() << " total" << std::endl;

	return failed > 0 ? 1 : 0;
}

#endif //CHESSPP_TESTFRAMEWORK_H
