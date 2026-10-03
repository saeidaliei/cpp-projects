#include <condition_variable>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>

// A bounded blocking queue gives us a very simple form of back-pressure:
// producers wait when the queue is full instead of allocating without limit.
template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity) : capacity_(capacity) {}

    bool push(T value) {
        std::unique_lock lock(mutex_);
        not_full_.wait(lock, [this] { return closed_ || queue_.size() < capacity_; });
        if (closed_) return false;
        queue_.push(std::move(value));
        not_empty_.notify_one();
        return true;
    }

    std::optional<T> pop() {
        std::unique_lock lock(mutex_);
        not_empty_.wait(lock, [this] { return closed_ || !queue_.empty(); });
        if (queue_.empty()) return std::nullopt;
        T value = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return value;
    }

    void close() {
        {
            std::lock_guard lock(mutex_);
            closed_ = true;
        }
        not_empty_.notify_all();
        not_full_.notify_all();
    }

private:
    std::size_t capacity_;
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    bool closed_{};
};

int main() {
    BoundedQueue<std::string> stage1(4);
    BoundedQueue<std::string> stage2(4);
    std::jthread producer([&] {
        for (int i = 1; i <= 20; ++i) {
            if (!stage1.push("record-" + std::to_string(i))) break;
        }
        stage1.close();
    });

    std::jthread transformer([&] {
        while (auto item = stage1.pop()) {
            for (char& ch : *item) {
                if (ch >= 'a' && ch <= 'z') ch = static_cast<char>(ch - 'a' + 'A');
            }
            if (!stage2.push(std::move(*item))) break;
        }
        stage2.close();
    });

    std::jthread consumer([&] {
        std::size_t count = 0;
        while (auto item = stage2.pop()) {
            std::cout << "consumed: " << *item << '\n';
            ++count;
        }
        std::cout << "Processed " << count << " records\n";
    });
}
