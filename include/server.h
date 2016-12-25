#pragma once
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "dao.h"
#include <string>
#include <map>

struct Server {
	Server(unsigned short port);
	~Server();
	void run();
private:
    typedef websocketpp::server<websocketpp::config::asio> ws_server;
    typedef ws_server::message_ptr message_ptr;
    ws_server bottleserve;
    Dao dao;
    // TODO: comparator thingy:
	// std::map<websocketpp::connection_hdl, DocumentId> connections;
};
