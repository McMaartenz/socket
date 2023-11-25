#include <server.h>

Server::Server(uint16_t port, std::filesystem::path root) : Socket(port), root{root} {}

void Server::map_static(std::string route, std::filesystem::path const& file) {
        if (!std::filesystem::exists(root / file)) {
                std::cerr << "File not mapped: " << file << std::endl;
                return;
        }

        routes[route] = root / file;
}

void Server::map_static_bulk(std::initializer_list<std::pair<std::string, std::filesystem::path>> routes) {
        for (auto& route : routes) {
                map_static(route.first, route.second);
        }
}

void Server::unmap_static(std::string route) {
        routes.erase(route);
}

void Server::map(Method method, std::string route, HttpHandler handler) {
        auto route_map = route_handlers.find(route);
        if (route_map == route_handlers.end()) {
                route_handlers.insert({route, {}});
                route_map = route_handlers.find(route);
        }

        auto& handler_map = route_map->second;
        auto stored_handler = handler_map.find(method);
        if (stored_handler != handler_map.end()) {
                std::cerr << "Handler already stored for: " << Method_to_string(method) << ' ' << route << std::endl;
                return;
        }

        handler_map.insert({method, handler});
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
                        response.headers["Allow"] = "GET";
                        break;
                }

                case Method::GET: {
                        std::string route = req.request_line.path.string();

                        if (auto path = routes.find(route); path != routes.end()) {
                                response = HttpRequest(HTTP::Status::OK, read_to_vector(path->second));
                                std::cout << '[' << httpCon.ip() << ':' << httpCon.port() << "] gave: " << path->first << std::endl;
                        } else {
                                response = HttpRequest(HTTP::Status::NOT_FOUND, read_to_vector(root / "404.html"));
                                std::cout << '[' << httpCon.ip() << ':' << httpCon.port() << "] gave: 404\n";
                        }
                        break;
                }

                default: {
                        response = HttpRequest(HTTP::Status::NOT_IMPLEMENTED);
                        break;
                }
                }

                httpCon.put_data(response);
        }
}

