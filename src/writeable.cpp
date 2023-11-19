#include <writeable.h>

std::string Writeable::receive() {
        char buffer[1024] {};
        int n = ::read(socket_fd, buffer, sizeof(buffer));
        return std::string(buffer, n);
}

Writeable::Writeable() : Writeable(0) {}
Writeable::Writeable(int socket_fd) : socket_fd{socket_fd} {}

void Writeable::send(std::string const& data) {
        ::send(socket_fd, data.c_str(), data.length(), 0);
}

