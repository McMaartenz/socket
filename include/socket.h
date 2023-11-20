#ifndef H__SOCKET
#define H__SOCKET

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <functional>

#include <writeable.h>
#include <connection.h>
#include <exception.h>

class Socket : public Writeable {
public:
        sockaddr_in address;

        Socket(uint16_t port);
        ~Socket();

        void bind_and_listen(int queue_length);
        void accept(std::function<void(Connection&)> handler);
};

#endif

