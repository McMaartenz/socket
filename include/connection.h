#ifndef H__CONNECTION
#define H__CONNECTION

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#include <writeable.h>

class Connection : public Writeable {
public:
        sockaddr_in client_address;

        Connection(int socket_fd, sockaddr_in client_address);

        void close();
        std::string ip() const;
        uint16_t port() const;
};

#endif

