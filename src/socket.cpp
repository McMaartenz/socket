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

