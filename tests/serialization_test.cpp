#include "gtest/gtest.h"
#include "serialization.h"

using namespace std;

class SerializationTest : public ::testing::Test {
    void SetUp() {
        game.id = "ABCD-1234";
        game.player1 = "player1";
        game.player2 = "player2";
        game.status = Game::Status::PLAYER1_SETTING_UP;

        profile.id = "BCDE-2345";
        profile.name = "player name";
        profile.password = "plaintext password";
    }
protected:
    Serializer serializer;
    Game game;
    Profile profile;
};

TEST_F(SerializationTest, serializes_profile_id) {
    auto serialized = serializer.from(profile);
    auto deserialized = serializer.to_profile(serialized);

    ASSERT_EQ(profile.id, deserialized.id);
}

TEST_F(SerializationTest, serializes_profile_name) {
    auto serialized = serializer.from(profile);
    auto deserialized = serializer.to_profile(serialized);

    ASSERT_EQ(profile.name, deserialized.name);
}

TEST_F(SerializationTest, serializes_profile_password) {
    auto serialized = serializer.from(profile);
    auto deserialized = serializer.to_profile(serialized);

    ASSERT_EQ(profile.password, deserialized.password);
}

TEST_F(SerializationTest, serializes_game_id) {
    auto serialized = serializer.from(game);
    auto deserialized = serializer.to_game(serialized);

    ASSERT_EQ(game.id, deserialized.id);
}

TEST_F(SerializationTest, serializes_player1) {
    auto serialized = serializer.from(game);
    auto deserialized = serializer.to_game(serialized);

    ASSERT_EQ(game.player1, deserialized.player1);
}

TEST_F(SerializationTest, serializes_player2) {
    auto serialized = serializer.from(game);
    auto deserialized = serializer.to_game(serialized);

    ASSERT_EQ(game.player2, deserialized.player2);
}

TEST_F(SerializationTest, serializes_status) {
    auto serialized = serializer.from(game);
    auto deserialized = serializer.to_game(serialized);

    ASSERT_EQ(game.status, deserialized.status);
}
