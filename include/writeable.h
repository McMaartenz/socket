#ifndef H__WRITEABLE
#define H__WRITEABLE

#include <sys/socket.h>
#include <unistd.h>
#include <vector>

/// A writeable stream
class Writeable {
public:
        int socket_fd;

        Writeable();
        Writeable(int socket_fd);

        /// Receive a buffer of data
        std::vector<char> receive();

        /// Send a buffer of data
        void send(std::vector<char> const& data);
};

#endif

