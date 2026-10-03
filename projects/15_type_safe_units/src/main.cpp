#include <concepts>
#include <iostream>
#include <ratio>
#include <type_traits>

// Unit is represented as "how many base units does one unit contain?".
template <typename Rep, typename Ratio>
requires std::is_arithmetic_v<Rep>
class Quantity {
public:
    constexpr explicit Quantity(Rep value) : value_(value) {}

    constexpr Rep count() const noexcept { return value_; }

    template <typename TargetRatio>
    constexpr auto to() const {
        using Factor = std::ratio_divide<Ratio, TargetRatio>;
        using Result = std::common_type_t<Rep, long double>;
        return Quantity<Result, TargetRatio>(
            static_cast<Result>(value_) * static_cast<Result>(Factor::num) /
            static_cast<Result>(Factor::den));
    }

private:
    Rep value_;
};

using Meter = Quantity<long double, std::ratio<1>>;
using Kilometer = Quantity<long double, std::kilo>;
using Second = Quantity<long double, std::ratio<1>>;
using Minute = Quantity<long double, std::ratio<60>>;

constexpr Kilometer operator""_km(long double value) {
    return Kilometer(value);
}

constexpr Minute operator""_min(long double value) {
    return Minute(value);
}

template <typename R1, typename R2>
constexpr auto add(Quantity<long double, R1> a, Quantity<long double, R2> b) {
    auto converted = b.template to<R1>();
    return Quantity<long double, R1>(a.count() + converted.count());
}

template <typename R1, typename R2>
constexpr auto divide(Quantity<long double, R1> distance,
                      Quantity<long double, R2> time) {
    return distance.count() / time.count();
}

int main() {
    const Kilometer trip = 3.5_km;
    const Meter meters = trip.to<std::ratio<1>>();
    const Minute duration = 4.0_min;
    const Second seconds = duration.to<std::ratio<1>>();

    std::cout << "Trip: " << meters.count() << " m\n";
    std::cout << "Duration: " << seconds.count() << " s\n";
    std::cout << "Average speed: " << divide(meters, seconds) << " m/s\n";

    constexpr auto total = add(Meter(500), Kilometer(1));
    static_assert(total.count() == 1500.0L);
    std::cout << "Total distance: " << total.count() << " m\n";
}
