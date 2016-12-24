#include "gtest/gtest.h"
#include "serialization.h"

class SerializationTest : public ::testing::Test {
    void SetUp() {
        game.id = "ABCD-1234";
        game.player1 = "player1";
        game.player2 = "player2";
        game.status = Game::Status::PLAYER1_SETTING_UP;
    }
protected:
    Serializer serializer;
    Game game;
};

TEST_F(SerializationTest, serializes_game_id) {
    auto serialized = serializer.from(game);
    auto deserialized = serializer.to_game(serialized);

    ASSERT_EQ(game.id, deserialized.id);
}
