#include "dao.h"

// LevelDB examples
#include "leveldb/db.h"
#include <iostream>
#include <sstream>

// JSON examples
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

using namespace std;
namespace {
    void json_stuff() {
        // Write json.
        ptree pt;
        pt.put ("foo", "bar");
        std::ostringstream buf;
        write_json (buf, pt, false);
        std::string json = buf.str(); // {"foo":"bar"}

        // Read json.
        ptree pt2;
        std::istringstream is (json);
        read_json (is, pt2);
        std::string foo = pt2.get<std::string> ("foo");
    }

    std::string map2json (const std::map<std::string, std::string>& map) {
        ptree pt;
        for (auto& entry: map)
            pt.put (entry.first, entry.second);
        std::ostringstream buf;
        write_json (buf, pt, false);
        return buf.str();
    }


    void check_status(leveldb::Status &status, const std::string& db_name) {
        if (!status.ok())
        {
            cerr << "Unable to open/create " << db_name << endl;
            cerr << status.ToString() << endl;
            throw exception();
        }
    }
}

/*
 * TODO: As a provisional approach, we can use LevelDB as a documentDB. Keys can be DocumentIds, values
 * can be JSON representations of the objects. All the classes in common.h map very easily to JSON, so it
 * should be straightforward:
 *
 */

struct DaoImpl {
    DaoImpl(const string &game_file, const string &profile_file) {
        leveldb::Options options;
        options.create_if_missing = true;

        auto profile_status = leveldb::DB::Open(options, profile_file, &profile_db);
        check_status(profile_status, profile_file);

        auto game_status = leveldb::DB::Open(options, game_file, &game_db);
        check_status(game_status, game_file);
    }
    ~DaoImpl() {
        if(profile_db) {
            delete profile_db;
        }
        if(game_db) {
            delete game_db;
        }
    }

    void update_profile(DocumentId id, const std::string &value) {
        profile_db->Put(write_options, to_string(id), value);
    }

    std::string get_profile(DocumentId id) {
        std::string result;
        profile_db->Get(read_options, to_string(id), &result);
        return result;
    }

    leveldb::ReadOptions read_options;
    leveldb::WriteOptions write_options;
    leveldb::DB *profile_db, *game_db;
};

DocumentId Dao::update_game(const Game &game) {
    //TODO
    return 0;
}

DocumentId Dao::update_profile(const Profile &profile) {
    std::string profile_json;
    //TODO: Serialize game...
    //TODO: Deal with uninitialized game id, i.e. a new profile
    impl->update_profile(profile.id, profile_json);
    return 0;
}

Game Dao::get_game(DocumentId id) {
    return Game();
}

Profile Dao::get_profile(DocumentId id) {
    auto profile_json = impl->get_profile(id);
    //TODO: Deerialize profile...
    return Profile();
}

Dao::~Dao() {

}

Dao::Dao(const string &game_file, const string &profile_file) {
    impl = make_unique<DaoImpl>(game_file, profile_file); //TODO: Probably makes sense to split out into two impl's, one for profile, one for game.
}
