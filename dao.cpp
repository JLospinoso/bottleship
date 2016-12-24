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

// Document IDs (UUIDs)
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

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

    struct LevelDb {
        LevelDb(const string& file) : name(file) {
            leveldb::Options options;
            options.create_if_missing = true;
            check_status(leveldb::DB::Open(options, file, &db), "LevelDb()");
        }
        ~LevelDb(){
            delete db;
        }
        std::string read(DocumentId id) {
            string result;
            check_status(db->Get(read_options, id, &result), "read");
            return result;
        }
        void update(DocumentId id, const std::string &value) {
            check_status(db->Put(write_options, id, value), "update");
        }
        void remove(DocumentId id) {
            check_status(db->Delete(write_options, id), "remove");
        }
        void check_status(const leveldb::Status &status, const string &info) const {
            if (!status.ok())
            {
                string err("(");
                err.append(name);
                err.append(name);
                err.append(") ");
                err.append(info);
                err.append(": ");
                err.append(status.ToString());
                throw DaoException(info);
            }
        }
    private:
        LevelDb(const LevelDb&) = delete;
        LevelDb& operator=(const LevelDb&) = delete;
        leveldb::DB *db;
        leveldb::ReadOptions read_options;
        leveldb::WriteOptions write_options;
        const string name;
    };
}

/*
 * TODO: As a provisional approach, we can use LevelDB as a documentDB. Keys can be DocumentIds, values
 * can be JSON representations of the objects. All the classes in common.h map very easily to JSON, so it
 * should be straightforward:
 *
 */

struct DaoImpl {
    DaoImpl(const string &game_file, const string &profile_file)
            : profiles(profile_file), games(game_file) {
    }
    Profile& save_profile(Profile& profile) {
        if(profile.id.empty()) {
            profile.id = to_string(uuid_gen());
        }
        string serialized_profile;
        //TODO: Serialization
        profiles.update(profile.id, serialized_profile);
        return profile;
    }
    Profile get_profile(DocumentId id) {
        Profile result;
        auto serialized_profile = profiles.read(id);
        //TODO: Deserialize
        return Profile();
    }
    Game& save_game(Game& game) {
        if(game.id.empty()) {
            game.id = to_string(uuid_gen());
        }
        string serialized_game;
        //TODO: Serialization
        games.update(game.id, serialized_game);
        return game;
    }
    Game get_game(DocumentId id) {
        Game result;
        auto serialized_game = games.read(id);
        //TODO: Deserialize
        return Game();
    }
private:
    boost::uuids::random_generator uuid_gen;
    LevelDb profiles, games;
};

Game& Dao::update_game(Game &game) {
    return impl->save_game(game);
}

Profile& Dao::update_profile(Profile &profile) {
    return impl->save_profile(profile);
}

Game Dao::get_game(DocumentId id) {
    return impl->get_game(id);
}

Profile Dao::get_profile(DocumentId id) {
    return impl->get_profile(id);
}

Dao::~Dao() {
    if(impl != nullptr) {
        delete impl;
    }
}

Dao::Dao(const string &game_file, const string &profile_file) {
    impl = new DaoImpl(game_file, profile_file);
}

Dao::Dao(Dao &&other) {
    impl = other.impl;
    other.impl = nullptr;
}

Dao &Dao::operator=(Dao &&other) {
    if(impl != nullptr) {
        delete impl;
    }
    impl = other.impl;
    other.impl = nullptr;
}

DaoException::DaoException(const string &__arg) : runtime_error(__arg) {}
