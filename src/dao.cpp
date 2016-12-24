#include "dao.h"
#include "leveldb/db.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "serialization.h"
#include <mutex>

using namespace std;
namespace {
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
            lock_guard<mutex> lock(rw_mutex);
            string result;
            check_status(db->Get(read_options, id, &result), "read");
            return result;
        }
        void update(DocumentId id, const std::string &value) {
            lock_guard<mutex> lock(rw_mutex);
            check_status(db->Put(write_options, id, value), "update");
        }
        void remove(DocumentId id) {
            lock_guard<mutex> lock(rw_mutex);
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
        std::mutex rw_mutex;
    };
}

struct DaoImpl {
    DaoImpl(const string &game_file, const string &profile_file)
            : profiles(profile_file), games(game_file) {
    }
    Profile& save_profile(Profile& profile) {
        if(profile.id.empty()) {
            profile.id = to_string(uuid_gen());
        }
        auto serialized_profile = serializer.from(profile);
        profiles.update(profile.id, serialized_profile);
        return profile;
    }
    Profile get_profile(DocumentId id) {
        Profile result;
        auto serialized_profile = profiles.read(id);
        return serializer.to_profile(serialized_profile);
    }
    Game& save_game(Game& game) {
        if(game.id.empty()) {
            game.id = to_string(uuid_gen());
        }
        auto serialized_game = serializer.from(game);
        games.update(game.id, serialized_game);
        return game;
    }
    Game get_game(DocumentId id) {
        Game result;
        auto serialized_game = games.read(id);
        return serializer.to_game(serialized_game);
    }
private:
    boost::uuids::random_generator uuid_gen;
    LevelDb profiles, games;
    Serializer serializer;
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
