#ifndef H__WRITEABLE
#define H__WRITEABLE

#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Writeable {
public:
        int socket_fd;

        Writeable();
        Writeable(int socket_fd);

        std::vector<char> receive();
        void send(std::vector<char> const& data);
};

#endif

