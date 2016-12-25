#include <iostream>
#include <sstream>

#include "server.h"

using namespace std;
namespace {
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
}

Server::~Server() {
    bottleserve.stop_listening();
    bottleserve.stop();
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

        // Step 1: Check if `hdl` is known. Not sure if we want to store a map of DocumentId to HDL
        //         If `hdl` is known, user is already logged in and we can dispatch the message, e.g.
        // auto connection_iter = connections.find(hdl);
        // if(connection_iter != connections.end()) // ...
        // const auto& profile = connection_iter->second;
        // dispatch(hdl, profile, msg->get_payload());
        // return;

        // Step 2: Unknown connection. Attempt to log the user in:
        const auto & line = msg->get_payload();
        array<string,3> login;
        if(login.size() != split(line, ' ', login.size(), login.begin()) || login[0] != "login") {
            bottleserve.close(hdl, close::status::unsupported_data, "You must login.");
            return;
        }
        auto new_profile = dao.get_profile_by_name(login[1]);
        if(new_profile.password.empty()) {
            // DAO will return a new profile with empty fields (except user).
            // Fill in the given password, save, and return as valid login.
            new_profile.password = login[2];
            dao.update_profile(new_profile);
        }
        if(new_profile.password != login[2]) {
            bottleserve.close(hdl, close::status::unsupported_data, "Incorrect password.");
            return;
        }
        // store the hdl to DocumentId

        // Step 3: Just for echo testing...
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
