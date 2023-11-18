#ifndef H__SOCKET
#define H__SOCKET

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <functional>
#include <filesystem>
#include <fstream>

class Connection;

class Writeable {
public:
        int socket_fd;

        Writeable();
        Writeable(int socket_fd);

        std::string receive();
        void send(std::string const& data);
};

class Socket : public Writeable {
public:
        sockaddr_in address;

        Socket(uint16_t port);
        ~Socket();

        void bind_and_listen(int queue_length);
        void accept(std::function<void(Connection&)> handler);
};

class Connection : public Writeable {
public:
        sockaddr_in client_address;

        Connection(int socket_fd, sockaddr_in client_address);

        void close();
};

namespace HTTP {
        std::string create_response(int status_code, std::string const& page);
        std::string create_response(int status_code, std::filesystem::path const& path);
};

class SocketException : public std::exception {
public:
        SocketException();
        inline virtual char const* what() const noexcept {
                return "Socket Exception";
        }
};

class BindException : public std::exception {
public:
        BindException();
        inline virtual char const* what() const noexcept {
                return "Bind Exception";
        }
};

class ListenException : public std::exception {
public:
        ListenException();
        inline virtual char const* what() const noexcept {
                return "Listen Exception";
        }
};

class AcceptException : public std::exception {
public:
        AcceptException();
        inline virtual char const* what() const noexcept {
                return "Accept Exception";
        }
};

#endif

