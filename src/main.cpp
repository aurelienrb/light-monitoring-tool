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
	return std::string{}; // Error 404!
}

bool endsWith(const std::string & str, const std::string & text) {
	if (text.length() <= str.length()) {
		return (str.compare(str.length() - text.length(), text.length(), text) == 0);
	}
	return false;
}

std::string generateContentTypeFromFileExtension(const std::string & fileName) {
	if (endsWith(fileName, ".js")) {
		return "Content-Type: application/javascript\r\n";
	}
	else if (endsWith(fileName, ".css")) {
		return "Content-Type: text/css\r\n";
	}
	else if (endsWith(fileName, ".png")) {
		return "Content-Type: image/png\r\n";
	}
	else {
		return std::string{};
	}
}

std::string error404() {
	static const std::string msg404 = "<html><head></head><body><h1>Page not found!</h1></body></html>";
	static const std::string httpHeader =
		std::string{ "HTTP/1.1 200 OK\r\n" } +
		"Content-Length: " + std::to_string(msg404.length()) + "\r\n" +
		"Content-Type: text/html\r\n" +
		"\r\n" +
		msg404;
	return httpHeader;
}

int main() {
	HttpServer server(8080, 1);
	server.config.address = "127.0.0.1";

	server.resource["^/$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		const std::string result = findStaticFile("index.html");
		*response << "HTTP/1.1 200 OK\r\n"
			<< "Content-Length: " << result.length() << "\r\n"
			<< "Content-Type: " << "text/html" << "\r\n"
			<< "\r\n" << result;
	};

	server.resource["^/(.)+.html$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		const std::string relPath(request->path, 1); // remove the front '/'
		const std::string result = findStaticFile(relPath);
		if (!result.empty()) {
			*response << "HTTP/1.1 200 OK\r\n"
				<< "Content-Length: " << result.length() << "\r\n"
				<< "Content-Type: " << "text/html" << "\r\n"
				<< "\r\n" << result;
		}
		else {
			*response << error404();
		}
	};

	server.resource["^/(css|js|images)/(.)+$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		try {
			const std::string relPath(request->path, 1); // remove the front '/'
			const std::string result = findStaticFile(relPath);
			if (!result.empty()) {
				*response << "HTTP/1.1 200 OK\r\n"
					<< "Content-Length: " << result.length() << "\r\n"
					<< generateContentTypeFromFileExtension(relPath)
					<< "\r\n" << result;
			}
			else {
				*response << error404();
			}
		}
		catch (const std::exception & e) {
			std::cerr << "Error: " << e.what() << "\n";
		}
	};

	//server.resource["^/favicon.ico$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
	//}

	server.resource["^/api/cpu$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
		const std::string result = "[1, 1, 2, 3, 5, 8]";
		*response << "HTTP/1.1 200 OK\r\n"
			<< "Content-Length: " << result.length() << "\r\n"
			<< "Content-Type: " << "application/json" << "\r\n"
			<< "\r\n" << result;
	};

	server.start();
}
