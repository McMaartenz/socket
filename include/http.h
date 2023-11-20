#ifndef H__HTTP
#define H__HTTP

#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <variant>

#include <connection.h>

enum class Method {
        UNSUPPORTED,
        GET
};

std::string Method_to_string(Method method);

namespace HTTP {
enum Status {
        // 200
        OK = 200,
        
        // 400
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,

        // 500
        NOT_IMPLEMENTED = 501
};
}

/// The request line, like GET / HTTP/1.1, or HTTP/1.1 200 OK
struct RequestLine {
        std::variant<Method, HTTP::Status> method, status_code;
        std::filesystem::path path;
        float http_version;
};

/// A single request from either client or server, single use
class HttpRequest {
public:
        RequestLine request_line;
        std::map<std::string, std::string> headers;
        std::vector<char> data;

        /// Create a new request
        HttpRequest();

        /** Create a new request
         * @param status_code HTTP status code to return
         */
        HttpRequest(HTTP::Status status_code);

        /** Create a new request
         * @param status_code HTTP status code to return
         * @param data Data to transmit with request
         */
        HttpRequest(HTTP::Status status_code, std::vector<char> const& data);
};

using HttpResponse = HttpRequest;

/// Manages several HTTP Requests
class HttpConnection : public Connection {
public:
        HttpRequest request;

        /** Create a new connection manager
         * @param socket_fd Socket file descriptor
         * @param client_address Remote address
         */
        HttpConnection(int socket_fd, sockaddr_in client_address);  

        /// Perform a read of the remote socket and parse it to an HTTP request object
        HttpRequest get_data();

        /// Transmit a request to the remote socket
        void put_data(HttpRequest const& request);
};

namespace HTTP {
        /// Parse a request line into its object
        RequestLine parse_request_line(std::string const& request_line);
};

#endif

