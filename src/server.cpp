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

                if (method == Method::UNSUPPORTED) {
                        response = HttpResponse(HTTP::Status::METHOD_NOT_ALLOWED);
                        response.headers["Allow"] = "GET";
                        httpCon.put_data(response);
                        continue;
                } 

                // Find function handler
                std::string route = req.request_line.path.string();
                if (auto path = route_handlers.find(route); path != route_handlers.end()) {
                        auto& mapped_handlers = path->second;
                        if (auto stored_handler = mapped_handlers.find(method); stored_handler != mapped_handlers.end()) {
                                response = (stored_handler->second)(req);
                        } else {
                                // Print other available handlers for route (Excepting static)
                                response = HttpResponse(HTTP::Status::METHOD_NOT_ALLOWED);
                                std::string allowed_headers = "";
                                for (auto it = mapped_handlers.begin(); it != mapped_handlers.end(); it++) {
                                        if (!allowed_headers.empty()) {
                                                allowed_headers += ", ";
                                        }

                                        allowed_headers += Method_to_string(it->first);
                                }

                                response.headers["Allow"] = allowed_headers;
                        }
                        
                        httpCon.put_data(response);
                        continue;
                }

                // Find static handler
                if (auto path = routes.find(route); method == Method::GET && path != routes.end()) {
                        response = HttpResponse(HTTP::Status::OK, read_to_vector(path->second)); 
                        std::cout << '[' << httpCon.ip() << ':' << httpCon.port() << "] gave: " << path->first << std::endl;
                } else {
                        response = HttpResponse(HTTP::Status::NOT_FOUND, read_to_vector(root / "404.html"));
                        std::cout << '[' << httpCon.ip() << ':' << httpCon.port() << "] gave: 404\n";
                }

                httpCon.put_data(response);
                continue;
        }
}

