#ifndef H__CONNECTION
#define H__CONNECTION

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#include <writeable.h>

/// A simple connection tracking the remote address
class Connection : public Writeable {
public:
        sockaddr_in client_address;

        /** Create a connection
         * @param socket_fd Socket file descriptor
         * @param client_address Address of the client
         */
        Connection(int socket_fd, sockaddr_in client_address);

        /// Close the connection
        void close();

        /// Retrieve the remote IP in std::string format
        std::string ip() const;

        /// Retrieve the remote port
        uint16_t port() const;
};

#endif

