#include <server.h>
#include <http.h>

void handler(HttpConnection& con);

int main() {
        Server server(8000, "www");

        server.bulk_map({
                {"/", "index.html"},
                {"/index.html", "index.html"},
        });

        server.bind_and_listen(20);

        std::cout << "Listening on 8000\n";
        server.listen();
}

