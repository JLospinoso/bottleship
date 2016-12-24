#pragma once
#include "common.h"

struct Serializer {
    Profile to_profile(const std::string &serialized);
    Game to_game(const std::string &serialized);
    std::string from(const Profile& profile);
    std::string from(const Game& game);
};
