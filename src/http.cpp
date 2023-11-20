#include <http.h>

HttpConnection::HttpConnection(int socket_fd, sockaddr_in client_address) : Connection(socket_fd, client_address) {}

HttpRequest HttpConnection::get_data() {
        HttpRequest request;

        request.data = receive();
        auto& data = request.data;

        std::string request_line{};

        std::istringstream iss(&data[0]);
        if (!std::getline(iss, request_line)) {
                std::cerr << "Request line invalid or too long\n";
                return {};
        }

        request.request_line = HTTP::parse_request_line(request_line);

        // Read request headers
        while (true) {
                std::string line;
                bool ok = !!std::getline(iss, line);
                if (!ok) {
                        break;
                }

                std::cout << "Header " << line << std::endl;
        }
        this->request = request;
        return request;
}

void HttpConnection::put_data(HttpResponse const& response) {
}

RequestLine HTTP::parse_request_line(std::string const& request_line) {
        RequestLine result{};

        float version;
        std::string method, directory;
        std::istringstream iss(request_line);
        iss >> method >> directory;
        
        iss.ignore(16, '/');
        iss >> version;

        result.method = Method::UNSUPPORTED;
        if (method == "GET") {
                result.method = Method::GET;
        } 

        result.path = {directory};
        result.http_version = version;

        return result;
}

std::string HTTP::create_response(int status_code, std::string const& page) {
        std::ostringstream oss;
        oss << "HTTP/1.1 ";
        oss << status_code;
        oss << '\n';

        oss << "Date: 0\n";
        oss << "Content-Type: text/html; charset=UTF-8\n";
        oss << "Content-Length: ";
        oss << page.length();
        oss << '\n';

        oss << "Last-Modified: 0\n";
        oss << "Accept-Ranges: bytes\n";
        oss << "Server: Other/4.2 (Unix) (Debian/Linux)\n";
        oss << "Connection: close\n";

        oss << '\n';

        oss << page;

        return oss.str();
}

std::string HTTP::create_response(int status_code, std::filesystem::path const& path) {
        std::ifstream file{path};
        std::ostringstream oss;
        oss << file.rdbuf();

        return create_response(status_code, oss.str());
}

