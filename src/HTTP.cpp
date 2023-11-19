#include <HTTP.h>

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

