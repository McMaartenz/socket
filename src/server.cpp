#include <server.h>
#include <http.h>

Server::Server(uint16_t port, std::filesystem::path root) : Socket(port), root{root} {}

void Server::map(std::string route, std::filesystem::path const& file) {
        if (!std::filesystem::exists(root / file)) {
                std::cerr << "File not mapped: " << file << std::endl;
                return;
        }

        routes[route] = root / file;
}

void Server::unmap(std::string route) {
        routes.erase(route);
}

void Server::listen() {
        while (true) {
                Connection connection = accept();
                HttpConnection httpCon{connection.socket_fd, connection.client_address};
                httpCon.get_data();
                std::cout << "New connection\n";
        }
}

