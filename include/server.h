#ifndef H__SERVER
#define H__SERVER

#include <map>
#include <string>
#include <filesystem>
#include <functional>

#include <socket.h>
#include <http.h>
#include <file.h>

using HttpHandler = std::function<HttpResponse(HttpRequest&)>;

/// Manages and handles connections, with routes configured
class Server : public Socket {
public:
        /** Create a server instance
         * @param port Port listening on
         * @param root Webserver root '/'
         */
        Server(uint16_t port, std::filesystem::path root);

        /** Map a static file to a route
         * @param route Route to bind to
         * @param file File to serve
         */
        void map_static(std::string route, std::filesystem::path const& file);

        /** Map several static files
         * @see map_static
         */
        void map_static_bulk(std::initializer_list<std::pair<std::string, std::filesystem::path>> routes);

        /** Map a handler to a route
         * @param method HTTP method to accept
         * @param route Route to bind to
         * @param handler Procedure to handle request
         */
        void map(Method method, std::string route, HttpHandler handler);

        /// Unmap a bound file
        void unmap_static(std::string route);

        /// Start listening and accept incoming requests
        void listen();

protected:
        using Socket::accept;

private:
        std::filesystem::path root;
        std::map<std::string, std::filesystem::path> routes;
        std::map<std::string, std::map<Method, HttpHandler>> route_handlers;
};

#endif

