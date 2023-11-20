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

/// Manages connections
class Socket : public Writeable {
public:
        sockaddr_in address;

        /** Create a socket
         * @param port Port to listen on
         */
        Socket(uint16_t port);
        ~Socket();

        /** Bind and listen to the configured port
         * @param queue_length Length of max pending connections
         */
        void bind_and_listen(int queue_length);

        /// Accept a connection
        Connection accept();
};

#endif

