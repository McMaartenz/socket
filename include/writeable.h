#ifndef H__WRITEABLE
#define H__WRITEABLE

#include <sys/socket.h>
#include <unistd.h>
#include <string>

class Writeable {
public:
        int socket_fd;

        Writeable();
        Writeable(int socket_fd);

        std::string receive();
        void send(std::string const& data);
};

#endif

