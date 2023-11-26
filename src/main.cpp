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
        if (auto user_agent = req.headers.find("user-agent"); user_agent != req.headers.end()) {
                std::cout << "The user agent passed with this request is: " << user_agent->second << std::endl;
        } else {
                std::cout << "This request contained no user agent\n";
        }

        return HttpResponse(HTTP::Status::OK, read_to_vector("www/sample/index.html"));
}

