#include <server_http.hpp>
#include <staticfiles.h>
#include <iostream>

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

std::string findStaticFile(const std::string & fileName) {
	for (auto file : static_files::fileList()) {
		if (file.name() == fileName) {
			std::cout << "Serving " << fileName << " (" << file.fileDataSize << " bytes)\n";
			return file.content();
		}
	}
	return "Error 404!";
}

int main() {
	HttpServer server(8080, 1);
	server.config.address = "127.0.0.1";

	server.resource["^/$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		const std::string result = findStaticFile("index.html");
		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << result.length() << "\r\n\r\n" << result;
	};

	server.resource["^/(css|js|images)/(.)+$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		try {
			const std::string relPath(request->path, 1); // remove the front '/'
			const std::string result = findStaticFile(relPath);
			*response << "HTTP/1.1 200 OK\r\nContent-Length: " << result.length() << "\r\n\r\n" << result;
		}
		catch (const std::exception & e) {
			std::cerr << "Error: " << e.what() << "\n";
		}
	};

	//server.resource["^/favicon.ico$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
	//}

	server.start();
}
