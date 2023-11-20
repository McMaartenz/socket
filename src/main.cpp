#include <server.h>
#include <http.h>

void handler(HttpConnection& con);

int main() {
        Server server(8000, "www");

        server.map("/", "index.html");

        try {
                server.bind_and_listen(20);
        } catch (std::exception const& e) {
                std::cerr << e.what() << std::endl;
                return 1;
        }
        std::cout << "Listening on 8000\n";

        server.listen(handler);
}

void handler(HttpConnection& con) {
        HttpRequest& req = con.request;
        std::cout << "New connection from " << con.ip() << ':' << con.port()
                  << " with version HTTP/" << req.request_line.http_version
                  << " for directory: " << req.request_line.path << std::endl;

        con.close();
}

