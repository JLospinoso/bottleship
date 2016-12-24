#include "gtest/gtest.h"
#include "dao.h"
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>

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

TEST_F(DaoTest, dao_can_retrieve_profile_by_name) {
    Dao dao(get_temp(), get_temp());
    Profile a, b, c;
    a.name = "Name A";
    b.name = "Name B";
    c.name = "Name C";
    dao.update_profile(a);
    dao.update_profile(b);
    dao.update_profile(c);

    auto result = dao.get_profile_by_name("Name B");

    ASSERT_EQ(b.id, result.id);
}

TEST_F(DaoTest, dao_can_retrieve_profile_by_name_with_old_db) {
    auto profiles_fn = get_temp();
    auto games_fn = get_temp();
    DocumentId expectedDocumentId;
    {
        Dao dao(profiles_fn, games_fn);
        Profile a, b, c;
        a.name = "Name A";
        b.name = "Name B";
        c.name = "Name C";
        dao.update_profile(a);
        dao.update_profile(b);
        dao.update_profile(c);
        expectedDocumentId = b.id;
    }

    Dao new_dao(profiles_fn, games_fn);
    Profile d;
    d.name = "Name D";
    auto result = new_dao.get_profile_by_name("Name B");

    ASSERT_EQ(expectedDocumentId, result.id);
}

TEST_F(DaoTest, dao_will_create_new_profile_for_new_name) {
    Dao dao(get_temp(), get_temp());
    auto profile = dao.get_profile_by_name("new name");

    auto retrieved = dao.get_profile(profile.id);

    ASSERT_EQ(profile.name, retrieved.name);
}

TEST_F(DaoTest, dao_can_iterate_over_profiles) {
    auto profiles_fn = get_temp();
    auto games_fn = get_temp();
    DocumentId expectedDocumentId;
    {
        Dao dao(profiles_fn, games_fn);
        Profile a, b, c;
        a.name = "Name A";
        b.name = "Name B";
        c.name = "Name C";
        dao.update_profile(a);
        dao.update_profile(b);
        dao.update_profile(c);
        expectedDocumentId = b.id;
    }

    Dao dao(profiles_fn, games_fn);
    Profile d;
    d.name = "Name D";
    dao.update_profile(d);
    vector<string> retrieved_names;
    dao.for_each_profile([&retrieved_names](const Profile& p){
        retrieved_names.push_back(p.name);
    });
    sort(retrieved_names.begin(), retrieved_names.end());
    ASSERT_EQ(vector<string>({"Name A", "Name B", "Name C", "Name D"}), retrieved_names);
}
