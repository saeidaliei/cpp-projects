#include <any>
#include <functional>
#include <iostream>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

class EventBus {
    using Callback = std::function<void(const std::any&)>;

public:
    template <typename Event, typename Fn>
    void subscribe(Fn&& fn) {
        auto& list = handlers_[std::type_index(typeid(Event))];
        list.emplace_back([callback = std::forward<Fn>(fn)](const std::any& value) {
            callback(std::any_cast<const Event&>(value));
        });
    }

    template <typename Event>
    void publish(const Event& event) const {
        const auto it = handlers_.find(std::type_index(typeid(Event)));
        if (it == handlers_.end()) {
            return;
        }
        const std::any erased = event;
        for (const Callback& callback : it->second) {
            callback(erased);
        }
    }

private:
    std::unordered_map<std::type_index, std::vector<Callback>> handlers_;
};

struct UserRegistered {
    std::string username;
};

struct PurchaseCompleted {
    std::string username;
    int cents{};
};

int main() {
    EventBus bus;

    bus.subscribe<UserRegistered>([](const UserRegistered& event) {
        std::cout << "Welcome " << event.username << "\n";
    });

    bus.subscribe<PurchaseCompleted>([](const PurchaseCompleted& event) {
        std::cout << event.username << " paid " << event.cents << " cents\n";
    });

    bus.publish(UserRegistered{"Ada"});
    bus.publish(PurchaseCompleted{"Ada", 1599});
}
