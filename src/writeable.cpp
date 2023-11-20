#include <writeable.h>

std::vector<char> Writeable::receive() {
        char buffer[4096] {};
        int n = ::read(socket_fd, buffer, sizeof(buffer));
        return std::vector(buffer, buffer + n);
}

Writeable::Writeable() : Writeable(0) {}
Writeable::Writeable(int socket_fd) : socket_fd{socket_fd} {}

void Writeable::send(std::vector<char> const& data) {
        ::send(socket_fd, &data[0], data.size(), 0);
}

