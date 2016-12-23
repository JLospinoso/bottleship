#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

class Server {
	typedef websocketpp::server<websocketpp::config::asio> ws_server;
	typedef ws_server::message_ptr message_ptr;

	ws_server bottleserve;
public:
	Server(unsigned short port);
	void run();
};
