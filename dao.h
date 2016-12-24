#pragma once
#include "common.h"
#include <memory>

struct DaoImpl;

struct Dao {
    Dao(const std::string& game_file="games.db", const std::string& profile_file="profiles.db");
    ~Dao();
    Dao(const Dao&) = delete;
    Dao& operator=(const Dao&) = delete;
    //TODO: Implement move

    DocumentId update_game(const Game& game);
    DocumentId update_profile(const Profile& game);
    Game get_game(DocumentId id);
    Profile get_profile(DocumentId id);

    /*
     * TODO: Implement something like:
    InputIterator<Profile> get_profiles();
    InputIterator<Game> get_profiles();
     */
private:
    std::unique_ptr<DaoImpl> impl; // PIMPL idiom
};
