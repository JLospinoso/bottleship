#include "serialization.h"
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

Profile Serializer::to_profile(const string &serialized) {
    ptree tree;
    read_json(serialized, tree);
    Profile result;
    result.id = tree.get<string>("id");
    result.name = tree.get<string>("name");
    result.password = tree.get<string>("password");
    //TODO: Games
    return result;
}

string Serializer::from(const Profile &profile) {
    ptree tree;
    tree.put("id", profile.id);
    tree.put("name", profile.name);
    tree.put("password", profile.password);
    //TODO: Games
    ostringstream stream;
    write_json(stream, tree, false);
    return stream.str();
}

Game Serializer::to_game(const std::string &serialized) {
    ptree tree;
    read_json(serialized, tree);
    Game result;
    result.id = tree.get<string>("id");
    result.player1 = tree.get<string>("player1");
    result.player2 = tree.get<string>("player2");
    result.status = Game::Status(tree.get<int>("status"));
    //TODO: Boards
    //TODO: Shots
    return result;
}

std::string Serializer::from(const Game &game) {
    ptree tree;
    tree.put("id", game.id);
    tree.put("player1", game.player1);
    tree.put("player2", game.player2);
    tree.put("status", game.status);
    //TODO: Boards
    //TODO: Shots
    ostringstream stream;
    write_json(stream, tree, false);
    return stream.str();
}
