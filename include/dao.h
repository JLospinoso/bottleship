#pragma once
#include "common.h"
#include <stdexcept>
#include <functional>

struct DaoImpl;

struct DaoException : public std::runtime_error {
    DaoException(const std::string&);
};

struct Dao {
    Dao(const std::string& game_file="games.db", const std::string& profile_file="profiles.db");
    Dao(const Dao&) = delete;
    Dao& operator=(const Dao&) = delete;
    Dao(Dao&&);
    Dao& operator=(Dao&&);
    ~Dao();

    Game& update_game(Game& game);
    Profile& update_profile(Profile& profile);
    Game get_game(DocumentId id);
    Profile get_profile(DocumentId id);
    Profile get_profile_by_name(const std::string& name);
    void for_each_profile(std::function<void(const Profile&)> fn);
    void for_each_game(std::function<void(const Game&)> fn);
private:
    DaoImpl *impl; // PIMPL idiom
};
