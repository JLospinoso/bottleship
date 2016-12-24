#pragma once
#include <string>
#include <vector>
#include <array>

typedef unsigned long long DocumentId;

struct Profile {
    DocumentId id;
    std::string name;
    std::string password;
    std::vector<DocumentId> games;
};

struct Board {
    // TODO: How to store board?
};

struct Shot {
    // TODO: How to store shot information?
};

struct Game {
    enum Status {
        CREATED=0,
        PLAYER1_SETTING_UP,
        PLAYER2_SETTING_UP,
        IN_PROGRESS,
        PLAYER1_VICTORY,
        PLAYER2_VICTORY
    } status;
    DocumentId id;
    std::string player1;
    Board boardPlayer1;
    std::string player2;
    Board boardPlayer2;
    std::vector<Shot> shotHistory;
};
