#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <stop_token>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(std::size_t worker_count) {
        workers_.reserve(worker_count);
        for (std::size_t i = 0; i < worker_count; ++i) {
            workers_.emplace_back([this](std::stop_token token) { worker(token); });
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard lock(mutex_);
            stopping_ = true;
        }
        for (auto& worker : workers_) {
            worker.request_stop();
        }
        cv_.notify_all();
        // std::jthread joins automatically after this destructor body returns.
    }

    template <typename Fn>
    auto submit(Fn&& fn) -> std::future<std::invoke_result_t<Fn>> {
        using Result = std::invoke_result_t<Fn>;
        std::packaged_task<Result()> task(std::forward<Fn>(fn));
        auto future = task.get_future();
        {
            std::lock_guard lock(mutex_);
            if (stopping_) {
                throw std::runtime_error("submit on stopped ThreadPool");
            }
            jobs_.push(std::move(task));
        }
        cv_.notify_one();
        return future;
    }

private:
    void worker(std::stop_token token) {
        while (true) {
            std::move_only_function<void()> job;
            {
                std::unique_lock lock(mutex_);
                cv_.wait(lock, token, [this] { return stopping_ || !jobs_.empty(); });
                if (jobs_.empty() && (stopping_ || token.stop_requested())) {
                    return;
                }
                job = std::move(jobs_.front());
                jobs_.pop();
            }
            job();
        }
    }

    std::vector<std::jthread> workers_;
    std::queue<std::move_only_function<void()>> jobs_;
    std::mutex mutex_;
    std::condition_variable_any cv_;
    bool stopping_{};
};

int main() {
    ThreadPool pool(4);
    std::vector<std::future<long long>> futures;

    for (int task_id = 1; task_id <= 8; ++task_id) {
        futures.push_back(pool.submit([task_id] {
            long long sum = 0;
            for (int i = 0; i < 2'000'000; ++i) {
                sum += (static_cast<long long>(i) * task_id) % 97;
            }
            return sum;
        }));
    }

    long long total = 0;
    for (auto& future : futures) {
        total += future.get();
    }
    std::cout << "Combined result = " << total << '\n';
}
