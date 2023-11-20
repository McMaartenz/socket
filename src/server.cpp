#include <server.h>

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

void Server::listen(std::function<void(HttpConnection&)> handler) {
        while (true) {
                Connection connection = accept();
                HttpConnection httpCon{connection.socket_fd, connection.client_address};
                httpCon.get_data();

                std::string content{};

                HttpResponse response;
                response.headers = {
                        {"Server", "Other/1.0 (Unix) (Debian/Linux)"},
                        {"Connection", "close"}
                };

                response.request_line.status_code = HTTP::Status::OK;
                response.request_line.http_version = 1.1f;
 
                decltype(auto) path = routes.find(httpCon.request.request_line.path.string());
                if (path == routes.end()) {
                        response.request_line.status_code = HTTP::Status::NOT_FOUND;
                }

                httpCon.put_data(response);

                // handler(httpCon);
        }
}

