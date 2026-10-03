#include <cmath>
#include <concepts>
#include <iostream>
#include <numbers>
#include <type_traits>
#include <vector>

// Arithmetic is intentionally constrained here. This function is not meant to
// accept arbitrary user-defined types just because they happen to have '+'.
template <std::floating_point T>
struct Point {
    T x{};
    T y{};

    constexpr Point operator+(const Point& other) const {
        return {x + other.x, y + other.y};
    }

    constexpr Point operator/(T scalar) const {
        return {x / scalar, y / scalar};
    }
};

template <std::floating_point T>
constexpr T distance(Point<T> a, Point<T> b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

template <std::floating_point T>
constexpr T triangle_area(Point<T> a, Point<T> b, Point<T> c) {
    const T cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return std::abs(cross) / static_cast<T>(2);
}

template <std::floating_point T>
constexpr T circle_area(T radius) {
    return std::numbers::pi_v<T> * radius * radius;
}

int main() {
    using P = Point<double>;
    constexpr P a{0.0, 0.0};
    constexpr P b{3.0, 0.0};
    constexpr P c{0.0, 4.0};

    static_assert(circle_area(2.0) > 12.56 && circle_area(2.0) < 12.57);

    std::cout << "AB = " << distance(a, b) << '\n';
    std::cout << "Triangle area = " << triangle_area(a, b, c) << '\n';
    std::cout << "Circle area = " << circle_area(2.0) << '\n';

    const std::vector<P> points{{1, 2}, {3, 4}, {5, 6}};
    P sum{};
    for (const P point : points) {
        sum = sum + point;
    }
    const P center = sum / static_cast<double>(points.size());
    std::cout << "Average point = (" << center.x << ", " << center.y << ")\n";
}
