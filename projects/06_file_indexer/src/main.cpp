#include <filesystem>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

struct ExtensionStats {
    std::size_t files{};
    std::uintmax_t bytes{};
};

int main(int argc, char** argv) {
    const std::filesystem::path root = argc >= 2 ? argv[1] : ".";
    std::error_code ec;

    if (!std::filesystem::exists(root, ec)) {
        std::cerr << "Path does not exist: " << root << '\n';
        return 1;
    }

    std::map<std::string, ExtensionStats> stats;
    std::size_t directories = 0;

    std::filesystem::recursive_directory_iterator it(root, ec), end;
    if (ec) {
        std::cerr << "Could not traverse " << root << ": " << ec.message() << '\n';
        return 1;
    }

    for (; it != end; it.increment(ec)) {
        if (ec) {
            std::cerr << "Traversal warning: " << ec.message() << '\n';
            ec.clear();
            continue;
        }

        const auto& entry = *it;
        std::error_code status_ec;
        if (entry.is_directory(status_ec)) {
            ++directories;
            continue;
        }
        if (!entry.is_regular_file(status_ec)) {
            continue;
        }

        const auto size = entry.file_size(status_ec);
        if (status_ec) {
            continue;
        }

        const std::string extension = entry.path().extension().empty()
            ? "<no extension>"
            : entry.path().extension().string();
        auto& item = stats[extension];
        ++item.files;
        item.bytes += size;
    }

    std::size_t total_files = 0;
    std::uintmax_t total_bytes = 0;
    for (const auto& [extension, info] : stats) {
        total_files += info.files;
        total_bytes += info.bytes;
        std::cout << std::left << std::setw(18) << extension
                  << std::right << std::setw(8) << info.files
                  << " files  " << std::setw(12) << info.bytes << " bytes\n";
    }
    std::cout << "\nDirectories: " << directories
              << "\nFiles: " << total_files
              << "\nBytes: " << total_bytes << '\n';
}
