#include <server.h>

Server::Server(uint16_t port, std::filesystem::path root) : Socket(port), root{root} {}

void Server::map(std::string route, std::filesystem::path const& file) {
        if (!std::filesystem::exists(root / file)) {
                std::cerr << "File not mapped: " << file << std::endl;
                return;
        }

        routes[route] = root / file;
}

