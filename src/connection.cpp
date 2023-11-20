#include <connection.h>

Connection::Connection(int socket_fd, sockaddr_in client_address)
        : Writeable{socket_fd}, client_address{client_address} {}

void Connection::close() {
        ::close(socket_fd);
}

std::string Connection::ip() const {
        char result[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_address.sin_addr, result, INET_ADDRSTRLEN);
        return {result};
}

uint16_t Connection::port() const {
        return ntohs(client_address.sin_port);
}

