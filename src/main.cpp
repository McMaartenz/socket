#include <server.h>
#include <http.h>

HttpResponse sample(HttpRequest& req);

int main() {
        Server server(8000, "www");

        server.map_static_bulk({
                {"/", "index.html"},
        });

        server.map(Method::GET, "/sample", ::sample);

        server.bind_and_listen(20);

        std::cout << "Listening on 8000\n";
        server.listen();
}

HttpResponse sample(HttpRequest& req) {
        return HttpResponse(HTTP::Status::OK, read_to_vector("/sample/index.html"));
}

