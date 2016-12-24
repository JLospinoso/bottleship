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
