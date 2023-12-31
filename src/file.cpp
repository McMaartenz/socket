#include <file.h>

std::string read_to_string(std::filesystem::path path) {
        std::ifstream file(path);
        if (!file.good()) {
                return "No such file";
        }

        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
}

std::vector<char> read_to_vector(std::filesystem::path path) {
        std::string content = read_to_string(path);
        return std::vector<char>(content.begin(), content.end());
}

