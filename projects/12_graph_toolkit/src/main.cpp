#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Graph {
public:
    void add_edge(std::string from, std::string to, int weight) {
        edges_[std::move(from)].push_back({std::move(to), weight});
    }

    std::unordered_map<std::string, int> dijkstra(const std::string& source) const {
        using State = std::pair<int, std::string>;
        const int infinity = std::numeric_limits<int>::max();
        std::unordered_map<std::string, int> distance;
        for (const auto& [node, _] : edges_) distance[node] = infinity;
        distance[source] = 0;

        std::priority_queue<State, std::vector<State>, std::greater<>> frontier;
        frontier.push({0, source});

        while (!frontier.empty()) {
            const auto [current_distance, node] = frontier.top();
            frontier.pop();

            if (current_distance != distance[node]) continue; // stale heap entry
            const auto outgoing = edges_.find(node);
            if (outgoing == edges_.end()) continue;

            for (const auto& [next, weight] : outgoing->second) {
                const int candidate = current_distance + weight;
                if (!distance.contains(next) || candidate < distance[next]) {
                    distance[next] = candidate;
                    frontier.push({candidate, next});
                }
            }
        }
        return distance;
    }

    std::vector<std::string> bfs(const std::string& source) const {
        std::vector<std::string> order;
        std::queue<std::string> queue;
        std::unordered_map<std::string, bool> seen;
        queue.push(source);
        seen[source] = true;

        while (!queue.empty()) {
            std::string node = queue.front();
            queue.pop();
            order.push_back(node);
            const auto it = edges_.find(node);
            if (it == edges_.end()) continue;
            for (const auto& [next, _] : it->second) {
                if (!seen[next]) {
                    seen[next] = true;
                    queue.push(next);
                }
            }
        }
        return order;
    }

private:
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> edges_;
};

int main() {
    Graph graph;
    graph.add_edge("A", "B", 4);
    graph.add_edge("A", "C", 2);
    graph.add_edge("C", "B", 1);
    graph.add_edge("B", "D", 5);
    graph.add_edge("C", "D", 8);

    std::cout << "BFS: ";
    for (const auto& node : graph.bfs("A")) std::cout << node << ' ';
    std::cout << "\nDistances from A:\n";
    for (const auto& [node, distance] : graph.dijkstra("A")) {
        std::cout << "  " << node << " -> " << distance << '\n';
    }
}
