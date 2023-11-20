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

#include <connection.h>

enum class Method {
        UNSUPPORTED,
        GET
};

struct RequestLine {
        Method method;
        std::filesystem::path path;
        float http_version;
};

class HttpRequest {
public:
        RequestLine request_line;
        std::map<std::string, std::string> attributes;
        std::vector<char> data;
};

using HttpResponse = HttpRequest;

class HttpConnection : public Connection {
public:
      HttpConnection(int socket_fd, sockaddr_in client_address);  

      HttpRequest get_data();
      void put_data(HttpRequest const& request);
};

namespace HTTP {
        RequestLine parse_request_line(std::string const& request_line);

        std::string create_response(int status_code, std::string const& page);
        std::string create_response(int status_code, std::filesystem::path const& path);
};

#endif

