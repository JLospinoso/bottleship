#include <iostream>
#include <sstream>

#include "server.h"

using namespace std;

/*
 * Splits a string based on a delimiter character
 *
 * Won't split into more than 'max' strings. Returns the number of strings that were added
 */
template<typename Iter>
size_t split(const string& str, char val, size_t max, Iter out) {
    stringstream ss(str);
    string s;
    size_t len = 0;

    // I love the "goes to" operator (-->)
    while( max --> 0 ) {
        if(!getline(ss, s, max ? val : '\0')) {
            return len;
        }
        len++;
        *out++ = s;
    }
    return len;
}

bool valid_user(const string& user, const string& pass) {
    // TODO: Actually check the username and password here
    return user == "anon" || (user == "corny" && pass == "1234");
}

Server::Server(unsigned short port) {
    // logging settings
    bottleserve.set_access_channels(websocketpp::log::alevel::all);
    bottleserve.clear_access_channels(websocketpp::log::alevel::frame_payload);

    bottleserve.init_asio();

    bottleserve.set_message_handler([this] (websocketpp::connection_hdl hdl, message_ptr msg) {
        using namespace websocketpp;
        if(msg->get_opcode() != frame::opcode::text) {
            bottleserve.close(hdl, close::status::unsupported_data, "Only text data allowed");
            return;
        }
        const auto & line = msg->get_payload();

        array<string,3> login;
        if(login.size() != split(line, ' ', login.size(), login.begin())
           || login[0] != "login"
           || !valid_user(login[1], login[2])) {
            bottleserve.close(hdl, close::status::unsupported_data, "Invalid login");
            return;
        }

        // TODO: Create a connection

        bottleserve.send(hdl, msg->get_payload(), msg->get_opcode());
        cout << msg->get_opcode() << endl;
        cout << msg->get_raw_payload() << endl;
    });

    bottleserve.listen(port);
}

void Server::run() {
    bottleserve.start_accept();

    bottleserve.run();
}
