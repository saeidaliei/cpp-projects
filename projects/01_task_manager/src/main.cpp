#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <functional>
#include <utility>
#include <vector>

struct Task {
    int id{};
    std::string title;
    bool done{};
};

class TaskStore {
public:
    explicit TaskStore(std::filesystem::path file) : file_(std::move(file)) {
        load();
    }

    const std::vector<Task>& all() const noexcept { return tasks_; }

    int add(std::string title) {
        const int id = next_id_++;
        tasks_.push_back(Task{id, std::move(title), false});
        save();
        return id;
    }

    bool mark_done(int id) {
        const auto task = find(id);
        if (!task.has_value()) {
            return false;
        }
        task->get().done = true;
        save();
        return true;
    }

    bool remove(int id) {
        const auto before = tasks_.size();
        tasks_.erase(
            std::remove_if(tasks_.begin(), tasks_.end(),
                           [id](const Task& task) { return task.id == id; }),
            tasks_.end());
        if (tasks_.size() == before) {
            return false;
        }
        save();
        return true;
    }

private:
    // Returning an optional reference avoids copying a Task just to mutate it.
    std::optional<std::reference_wrapper<Task>> find(int id) {
        for (Task& task : tasks_) {
            if (task.id == id) {
                return task;
            }
        }
        return std::nullopt;
    }

    void load() {
        std::ifstream input(file_);
        if (!input) {
            return; // First run: there is simply no database yet.
        }

        std::string line;
        while (std::getline(input, line)) {
            std::istringstream row(line);
            Task task;
            int done = 0;
            if (row >> task.id >> done && row >> std::quoted(task.title)) {
                task.done = done != 0;
                tasks_.push_back(std::move(task));
                next_id_ = std::max(next_id_, task.id + 1);
            }
        }
    }

    void save() const {
        // Writing to a temporary file then renaming it is a simple way to avoid
        // leaving a half-written file if the process is interrupted mid-write.
        const auto temp = file_.string() + ".tmp";
        std::ofstream output(temp, std::ios::trunc);
        for (const Task& task : tasks_) {
            output << task.id << ' ' << (task.done ? 1 : 0) << ' '
                   << std::quoted(task.title) << '\n';
        }
        output.close();
        std::error_code ec;
        std::filesystem::rename(temp, file_, ec);
        if (ec) {
            // Windows can reject rename-over-existing. Falling back keeps the
            // example portable enough for practice without hiding the issue.
            std::filesystem::remove(file_, ec);
            std::filesystem::rename(temp, file_, ec);
        }
    }

    std::filesystem::path file_;
    std::vector<Task> tasks_;
    int next_id_{1};
};

void print_help() {
    std::cout << "Commands: list | add <title> | done <id> | remove <id> | help | quit\n";
}

int main() {
    TaskStore store("tasks.txt");
    std::cout << "Task Manager\n";
    print_help();

    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        std::istringstream input(line);
        std::string command;
        input >> command;

        if (command == "list") {
            for (const Task& task : store.all()) {
                std::cout << '[' << (task.done ? 'x' : ' ') << "] " << task.id
                          << ": " << task.title << '\n';
            }
        } else if (command == "add") {
            std::string title;
            std::getline(input >> std::ws, title);
            if (title.empty()) {
                std::cout << "A title is required.\n";
                continue;
            }
            std::cout << "Created task " << store.add(std::move(title)) << '\n';
        } else if (command == "done" || command == "remove") {
            int id{};
            if (!(input >> id)) {
                std::cout << "Expected an integer id.\n";
                continue;
            }
            const bool ok = command == "done" ? store.mark_done(id) : store.remove(id);
            std::cout << (ok ? "Updated.\n" : "Task not found.\n");
        } else if (command == "help") {
            print_help();
        } else if (command == "quit" || command == "exit") {
            break;
        } else if (!command.empty()) {
            std::cout << "Unknown command.\n";
        }
    }
}
