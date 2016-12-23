#include <iostream>

#include "server.h"

Server::Server(unsigned short port) {
	// logging settings
	bottleserve.set_access_channels(websocketpp::log::alevel::all);
	bottleserve.clear_access_channels(websocketpp::log::alevel::frame_payload);

	bottleserve.init_asio();

	bottleserve.set_message_handler([this] (websocketpp::connection_hdl hdl, message_ptr msg) {
		bottleserve.send(hdl, msg->get_payload(), msg->get_opcode());
	});

	bottleserve.listen(port);
}

void Server::run() {
	bottleserve.start_accept();

	bottleserve.run();
}
