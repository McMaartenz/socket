#include <http.h>

std::string Method_to_string(Method method) {
        switch (method) {
        case Method::UNSUPPORTED: return "Unsupported";
        case Method::GET: return "GET";
        default: return "NotImplemented";
        }
}

HttpConnection::HttpConnection(int socket_fd, sockaddr_in client_address) : Connection(socket_fd, client_address) {}

HttpRequest::HttpRequest() : HttpRequest(HTTP::Status::OK) {}

HttpRequest::HttpRequest(HTTP::Status status_code) : HttpRequest(status_code, {}) {}

HttpRequest::HttpRequest(HTTP::Status status_code, std::vector<char> const& data) {
        headers = {
                {"Server", "Other/1.0 (Unix) (Debian/Unix)"},
                {"Connection", "close"},
                {"Content-Length", std::to_string(data.size())}
        };

        if (!data.empty()) {
                headers["Content-Type"] = "text/html;charset=UTF-8";
        }

        request_line.status_code = status_code;
        request_line.http_version = 1.1f;

        this->data = data;
}

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
                if (!ok) break;
                if (line.empty() || line == "\r") break; // data comes now

                std::string header_key, header_val;
                std::istringstream line_iss(line);
                line_iss >> header_key >> header_val;

                header_key.pop_back();
                std::transform(header_key.begin(), header_key.end(), header_key.begin(), [](uint8_t c){ return std::tolower(c); });
                request.headers[header_key] = header_val;
        }

        this->request = request;
        return request;
}

void HttpConnection::put_data(HttpResponse const& response) {
        std::ostringstream out;
        out << "HTTP/" << response.request_line.http_version << ' ';
        out << (uint32_t)std::get<HTTP::Status>(response.request_line.status_code) << std::endl;

        for (decltype(auto) header : response.headers) {
                out << header.first << ": " << header.second << std::endl;
        }

        // Mark border between header, data
        out << std::endl;

        std::string joined = out.str();
        std::vector<char> data(joined.begin(), joined.end());
        data.insert(data.end(), response.data.begin(), response.data.end());

        send(data);
        close();
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

