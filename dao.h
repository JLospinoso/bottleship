#pragma once
#include "common.h"

struct DaoImpl;
struct DaoException : public std::runtime_error {
    DaoException(const std::string &__arg);

};
struct Dao {
    Dao(const std::string& game_file="games.db", const std::string& profile_file="profiles.db");
    Dao(const Dao&) = delete;
    Dao& operator=(const Dao&) = delete;
    Dao(Dao&&);
    Dao& operator=(Dao&&);
    ~Dao();

    Game& update_game(Game& game);
    Profile& update_profile(Profile& game);
    Game get_game(DocumentId id);
    Profile get_profile(DocumentId id);

    /*
     * TODO: Implement iterators. This is easily done on the LevelDB side and would be useful for analytics.
    InputIterator<Profile> get_profiles();
    InputIterator<Game> get_profiles();
     */
private:
    DaoImpl *impl; // PIMPL idiom
};
