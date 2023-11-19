#include <socket.h>

Socket::Socket(uint16_t port) {
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
}

Socket::~Socket() {
        close(socket_fd);
}

void Socket::bind_and_listen(int queue_length) {
        socket_fd = socket(PF_INET, SOCK_STREAM, 0);
        if (socket_fd == -1) {
                throw SocketException();
        }

        int bind_result = bind(socket_fd, (sockaddr*)&address, sizeof(sockaddr));
        if (bind_result == -1) {
                throw BindException();
        }

        int listen_result = listen(socket_fd, queue_length);
        if (listen_result == -1) {
                throw ListenException();
        }
}

void Socket::accept(std::function<void(Connection&)> handler) {
        sockaddr_in client_address;
        socklen_t length = sizeof(client_address);

        int conn_socket_fd = ::accept(socket_fd, (sockaddr*)&client_address, &length);
        if (conn_socket_fd == -1) {
                throw AcceptException();
        }

        Connection connection(conn_socket_fd, client_address);
        handler(connection);
}

Connection::Connection(int socket_fd, sockaddr_in client_address)
        : Writeable{socket_fd}, client_address{client_address} {}

void Connection::close() {
        ::close(socket_fd);
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

SocketException::SocketException() : std::exception() {}
BindException::BindException() : std::exception() {}
ListenException::ListenException() : std::exception() {}
AcceptException::AcceptException() : std::exception() {}

