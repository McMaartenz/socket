#include <connection.h>

Connection::Connection(int socket_fd, sockaddr_in client_address)
        : Writeable{socket_fd}, client_address{client_address} {}

void Connection::close() {
        ::close(socket_fd);
}

