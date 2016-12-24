#pragma once
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "dao.h"
#include <string>

struct Server {
	Server(unsigned short port);
	void run();
private:
    bool valid_user(const std::string& user, const std::string& pass);
    typedef websocketpp::server<websocketpp::config::asio> ws_server;
    typedef ws_server::message_ptr message_ptr;
    ws_server bottleserve;
    Dao dao;
};
