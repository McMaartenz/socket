#include <server.h>

Server::Server(uint16_t port, std::filesystem::path root) : Socket(port), root{root} {}

void Server::map(std::string route, std::filesystem::path const& file) {
        if (!std::filesystem::exists(root / file)) {
                std::cerr << "File not mapped: " << file << std::endl;
                return;
        }

        routes[route] = root / file;
}

void Server::bulk_map(std::initializer_list<std::pair<std::string, std::filesystem::path>> routes) {
        for (auto& route : routes) {
                map(route.first, route.second);
        }
}

void Server::unmap(std::string route) {
        routes.erase(route);
}

void Server::listen() {
        while (true) {
                Connection connection = accept();
                HttpConnection httpCon{connection.socket_fd, connection.client_address};
                httpCon.get_data();

                HttpRequest& req = httpCon.request;
                Method method = std::get<Method>(req.request_line.method);

                std::cout << '[' << httpCon.ip() << ':' << httpCon.port() << "] "
                          << Method_to_string(method) << ' '
                          << req.request_line.path << ' '
                          << "HTTP/" << req.request_line.http_version << std::endl;

                HttpResponse response;
                switch (method) {
                case Method::UNSUPPORTED: {
                        response = HttpResponse(HTTP::Status::METHOD_NOT_ALLOWED);
                        break;
                }

                case Method::GET: {
                        if (auto path = routes.find(req.request_line.path.string()); path != routes.end()) {
                                response = HttpRequest(HTTP::Status::OK, read_to_vector(path->second));
                        } else {
                                response = HttpRequest(HTTP::Status::NOT_FOUND, read_to_vector(root / "404.html"));
                        }
                        break;
                }

                default: {
                        response = HttpRequest(HTTP::Status::NOT_IMPLEMENTED);
                        break;
                }
                }

                httpCon.put_data(response);

                // handler(httpCon);
        }
}

