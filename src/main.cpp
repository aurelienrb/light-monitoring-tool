#include <server_http.hpp>

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

int main() {
	HttpServer server(8080, 1);
	server.config.address = "127.0.0.1";

	server.resource["^/$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		const std::string result = "OK!";
		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << result.length() << "\r\n\r\n" << result;
	};

	server.start();
}
