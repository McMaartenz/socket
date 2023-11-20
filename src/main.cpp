#include <server.h>
#include <http.h>

void handler(Connection& conn);

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

        server.listen();
}

void handler(Connection& conn) {
        std::cout << "New connection\n";
        conn.close();
}

