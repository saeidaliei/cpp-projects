#include <coroutine>
#include <exception>
#include <iterator>
#include <iostream>
#include <utility>

// This generator is intentionally tiny. Production coroutine types usually
// expose more iterator/category guarantees and more careful cancellation APIs.
template <typename T>
class Generator {
public:
    struct promise_type;
    using Handle = std::coroutine_handle<promise_type>;

    struct promise_type {
        T current{};
        std::exception_ptr exception;

        Generator get_return_object() { return Generator{Handle::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) noexcept {
            current = std::move(value);
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    explicit Generator(Handle handle) : handle_(handle) {}
    Generator(Generator&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    ~Generator() { if (handle_) handle_.destroy(); }

    class iterator {
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;

        iterator() = default;
        explicit iterator(Handle handle) : handle_(handle) { resume(); }

        const T& operator*() const { return handle_.promise().current; }
        iterator& operator++() { resume(); return *this; }
        bool operator==(std::default_sentinel_t) const { return !handle_ || handle_.done(); }

    private:
        void resume() {
            handle_.resume();
            if (handle_.done() && handle_.promise().exception) {
                std::rethrow_exception(handle_.promise().exception);
            }
        }
        Handle handle_{};
    };

    iterator begin() { return iterator{handle_}; }
    std::default_sentinel_t end() { return {}; }

private:
    Handle handle_{};
};

Generator<unsigned long long> fibonacci(std::size_t count) {
    unsigned long long a = 0;
    unsigned long long b = 1;
    for (std::size_t i = 0; i < count; ++i) {
        co_yield a;
        const auto next = a + b;
        a = b;
        b = next;
    }
}

int main() {
    for (const auto value : fibonacci(20)) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}
