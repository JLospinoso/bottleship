#include "dao.h"
#include "leveldb/db.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "serialization.h"
#include <mutex>
#include <map>
#include <iostream>

using namespace std;

namespace {
    struct LevelDb {
        LevelDb(const string& file) : name(file) {
            leveldb::Options options;
            options.create_if_missing = true;
            check_status(leveldb::DB::Open(options, file, &db), "LevelDb constructor");
        }
        ~LevelDb(){
            try {
                delete db;
            } catch (std::exception &e) {
                cerr << e.what() << endl;
            }
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
        leveldb::Iterator* iterate() {
            return db->NewIterator(read_options);
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
    struct LevelDbIter {
        LevelDbIter(leveldb::Iterator *iter) : iter(iter) {
            if(iter == nullptr) {
                throw DaoException("Iterator was null.");
            }
        }
        ~LevelDbIter() {
            if(nullptr != iter) {
                delete iter;
            }
        }
        void for_each(std::function<void(std::string, std::string)> fn) {
            for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
                fn(iter->key().ToString(), iter->value().ToString());
            }
        }
    private:
        leveldb::Iterator *iter;
    };
}

struct DaoImpl {
    DaoImpl(const string &game_file, const string &profile_file)
            : profiles(profile_file), games(game_file) {
        LevelDbIter iter(profiles.iterate());
        iter.for_each([this](string key, string value){
            auto profile = serializer.to_profile(value);
            profile_name_index[profile.name] = key;
        });
    }
    Profile get_or_create_profile_by_name(const string &name) {
        lock_guard<mutex> lock(rw_mutex);
        auto id_itr = profile_name_index.find(name);
        if(profile_name_index.end() == id_itr) {
            Profile result;
            result.name = name;
            result.id = to_string(uuid_gen());
            auto serialized_profile = serializer.from(result);
            profiles.update(result.id, serialized_profile);
            profile_name_index[name] = result.id;
            return result;
        }
        auto id = id_itr->second;
        auto serialized_profile = profiles.read(id);
        return serializer.to_profile(serialized_profile);
    }
    Profile& save_profile(Profile& profile) {
        lock_guard<mutex> lock(rw_mutex);
        if(profile.id.empty()) {
            profile.id = to_string(uuid_gen());
        }
        auto serialized_profile = serializer.from(profile);
        profiles.update(profile.id, serialized_profile);
        profile_name_index[profile.name] = profile.id;
        return profile;
    }
    Profile get_profile(DocumentId id) {
        lock_guard<mutex> lock(rw_mutex);
        Profile result;
        auto serialized_profile = profiles.read(id);
        return serializer.to_profile(serialized_profile);
    }
    Game& save_game(Game& game) {
        lock_guard<mutex> lock(rw_mutex);
        if(game.id.empty()) {
            game.id = to_string(uuid_gen());
        }
        auto serialized_game = serializer.from(game);
        games.update(game.id, serialized_game);
        return game;
    }
    Game get_game(DocumentId id) {
        lock_guard<mutex> lock(rw_mutex);
        Game result;
        auto serialized_game = games.read(id);
        return serializer.to_game(serialized_game);
    }
    void for_each_profile(function<void(const Profile &)> function) {
        LevelDbIter iter(profiles.iterate());
        iter.for_each([&function, this](const string& key, const string& value){
            function(serializer.to_profile(value));
        });
    }
    void for_each_game(function<void(const Game &)> function) {
        LevelDbIter iter(games.iterate());
        iter.for_each([&function, this](const string& key, const string& value){
            function(serializer.to_game(value));
        });
    }
private:
    boost::uuids::random_generator uuid_gen;
    LevelDb profiles, games;
    Serializer serializer;
    map<string, DocumentId> profile_name_index;
    std::mutex rw_mutex;
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

Profile Dao::get_profile_by_name(const std::string& name) {
    return impl->get_or_create_profile_by_name(name);
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

void Dao::for_each_profile(std::function<void(const Profile&)> fn) {
    impl->for_each_profile(fn);
}

void Dao::for_each_game(std::function<void(const Game&)> fn) {
    impl->for_each_game(fn);
}

DaoException::DaoException(const string &msg) : std::runtime_error(msg) {}
