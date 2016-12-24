#include "serialization.h"
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

namespace {
    void json_stuff() {
        // Write json.
        ptree pt;
        pt.put("foo", "bar");
        ostringstream buf;
        write_json(buf, pt, false);
        string json = buf.str(); // {"foo":"bar"}

        // Read json.
        ptree pt2;
        istringstream is(json);
        read_json(is, pt2);
        string foo = pt2.get<string>("foo");
    }

    string map2json(const map<string, string> &map) {
        ptree pt;
        for (auto &entry: map)
            pt.put(entry.first, entry.second);
        ostringstream buf;
        write_json(buf, pt, false);
        return buf.str();
    }
}

Profile Serializer::to_profile(const string &serialized) {
    ptree tree;
    //istringstream is(serialized);
    read_json(serialized, tree);
    Profile result;
    result.id = tree.get<string>("id");
    result.name = tree.get<string>("name");
    result.password = tree.get<string>("password");
    //TODO: Games
    return result;
}

string Serializer::from(const Profile &profile) {
    return "";
}

std::string Serializer::from(const Game &game) {
    return "";
}

Game Serializer::to_game(const std::string &serialized) {
    return Game();
}
