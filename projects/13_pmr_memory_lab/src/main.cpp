#include <array>
#include <iostream>
#include <memory_resource>
#include <string_view>
#include <vector>

class CountingResource : public std::pmr::memory_resource {
public:
    explicit CountingResource(std::pmr::memory_resource* upstream) : upstream_(upstream) {}

    std::size_t allocations() const noexcept { return allocations_; }
    std::size_t bytes() const noexcept { return bytes_; }

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        ++allocations_;
        bytes_ += bytes;
        return upstream_->allocate(bytes, alignment);
    }

    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
        upstream_->deallocate(p, bytes, alignment);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

private:
    std::pmr::memory_resource* upstream_;
    std::size_t allocations_{};
    std::size_t bytes_{};
};

int main() {
    std::array<std::byte, 4096> buffer{};
    std::pmr::monotonic_buffer_resource arena(buffer.data(), buffer.size());
    CountingResource counting(&arena);

    std::pmr::vector<std::pmr::string> words{&counting};
    words.reserve(32);

    for (std::string_view word : {"modern", "C++", "memory", "resource", "design"}) {
        // The inner string must receive the same allocator as the vector element
        // so its character storage also comes from the PMR resource.
        words.push_back(std::pmr::string(word, &counting));
    }

    for (const auto& word : words) {
        std::cout << word << ' ';
    }
    std::cout << "\nallocations = " << counting.allocations()
              << ", bytes requested = " << counting.bytes() << '\n';
}
