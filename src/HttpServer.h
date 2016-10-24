#pragma once

#include <memory>

class HttpServer {
public:
	HttpServer();
	~HttpServer();
	
	void start();

private:
	class Pimpl;
	std::unique_ptr<Pimpl> m_pimpl;
};
