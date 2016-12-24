#include "gtest/gtest.h"
#include "dao.h"
#include <boost/filesystem.hpp>

using namespace std;

class DaoTest : public ::testing::Test {
    void SetUp() {
        game.player1 = "player1";
        game.player2 = "player2";
        game.status = Game::Status::PLAYER2_VICTORY;

        profile.name = "player name";
        profile.password = "plaintext password";
    }
protected:
    string get_temp() const {
        return boost::filesystem::unique_path().native();
    }
    Game game;
    Profile profile;
};

TEST_F(DaoTest, dao_gives_game_an_id) {
    Dao dao(get_temp(), get_temp());

    dao.update_game(game);

    ASSERT_FALSE(game.id.empty());
}

TEST_F(DaoTest, dao_returns_game_ref) {
    Dao dao(get_temp(), get_temp());

    auto &result = dao.update_game(game);

    ASSERT_EQ(&game, &result);
}

TEST_F(DaoTest, dao_can_roundtrip_game) {
    Dao dao(get_temp(), get_temp());
    auto id = dao.update_game(game).id;

    auto result = dao.get_game(id);

    ASSERT_EQ(game.id, result.id);
    ASSERT_EQ(game.player1, result.player1);
    ASSERT_EQ(game.player2, result.player2);
    ASSERT_EQ(game.status, result.status);
}

TEST_F(DaoTest, dao_gives_profile_an_id) {
    Dao dao(get_temp(), get_temp());

    dao.update_profile(profile);

    ASSERT_FALSE(profile.id.empty());
}

TEST_F(DaoTest, dao_returns_profile_ref) {
    Dao dao(get_temp(), get_temp());

    auto &result = dao.update_profile(profile);

    ASSERT_EQ(&profile, &result);
}

TEST_F(DaoTest, dao_can_roundtrip_profile) {
    Dao dao(get_temp(), get_temp());
    auto id = dao.update_profile(profile).id;

    auto result = dao.get_profile(id);

    ASSERT_EQ(profile.id, result.id);
    ASSERT_EQ(profile.name, result.name);
    ASSERT_EQ(profile.password, result.password);
}
