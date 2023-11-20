#ifndef H__SERVER
#define H__SERVER

#include <map>
#include <string>
#include <filesystem>

#include <socket.h>
#include <http.h>
#include <file.h>

class Server : public Socket {
public:
        Server(uint16_t port, std::filesystem::path root);

        void map(std::string route, std::filesystem::path const& file);
        void unmap(std::string route);

        void listen();

protected:
        using Socket::accept;

private:
        std::filesystem::path root;
        std::map<std::string, std::filesystem::path> routes;
};

#endif

