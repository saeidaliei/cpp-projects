#include <array>
#include <atomic>
#include <cstddef>
#include <iostream>
#include <thread>
#include <type_traits>

// SPSC means exactly one thread calls push() and exactly one thread calls pop().
template <typename T, std::size_t Capacity>
requires (Capacity > 1 && std::is_nothrow_move_assignable_v<T>)
class SpscRingBuffer {
public:
    bool try_push(T value) noexcept {
        const std::size_t tail = tail_.load(std::memory_order_relaxed);
        const std::size_t next = increment(tail);
        if (next == head_.load(std::memory_order_acquire)) {
            return false; // The consumer has not freed the next slot yet.
        }

        buffer_[tail] = std::move(value);
        // Release publishes both the stored value and the new tail index.
        tail_.store(next, std::memory_order_release);
        return true;
    }

    bool try_pop(T& out) noexcept {
        const std::size_t head = head_.load(std::memory_order_relaxed);
        if (head == tail_.load(std::memory_order_acquire)) {
            return false;
        }

        out = std::move(buffer_[head]);
        // Release tells the producer that this slot is safe to reuse.
        head_.store(increment(head), std::memory_order_release);
        return true;
    }

private:
    static constexpr std::size_t increment(std::size_t value) noexcept {
        return (value + 1) % Capacity;
    }

    std::array<T, Capacity> buffer_{};
    std::atomic<std::size_t> head_{0};
    std::atomic<std::size_t> tail_{0};
};

int main() {
    constexpr std::size_t count = 100'000;
    SpscRingBuffer<std::size_t, 1024> queue;
    std::atomic<std::size_t> total{0};

    std::jthread consumer([&] {
        std::size_t value{};
        for (std::size_t seen = 0; seen < count;) {
            if (queue.try_pop(value)) {
                total.fetch_add(value, std::memory_order_relaxed);
                ++seen;
            } else {
                std::this_thread::yield();
            }
        }
    });

    std::jthread producer([&] {
        for (std::size_t value = 1; value <= count;) {
            if (queue.try_push(value)) {
                ++value;
            } else {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();

    const std::size_t expected = count * (count + 1) / 2;
    std::cout << "Total = " << total.load() << "\n"
              << "Expected = " << expected << '\n';
}
