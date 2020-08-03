#include <gtest/gtest.h>

#include "test-helpers.hh"

const std::vector<position> player_plant = {{10, 10}, {11, 11}};
const std::vector<position> player_plant_birth = {{17, 17}, {19, 19}};
const std::vector<position> player_plant_far = {{16, 10}, {17, 11}};
const std::vector<position> player_plant_too_far = {{17, 10}, {18, 11}};

TEST_F(ApiTest, test_action_baffer_ok)
{
    for (int player : {0, 1})
    {
        EXPECT_EQ(OK, p_api[player]->baffer(player_plant[player],
                                            player_plant[1 - player]));

        EXPECT_EQ(DEJA_BAFFEE, p_api[player]->baffer(player_plant[player],
                                                     player_plant[1 - player]));

        EXPECT_EQ(49,
                  p_api[player]->plante_sur_case(player_plant[1 - player]).vie);

        next_turn();
    }

    for (int player : {0, 1})
    {
        EXPECT_EQ(OK, p_api[player]->baffer(player_plant[player],
                                            player_plant_far[player]));

        next_turn();
    }

    EXPECT_EQ(OK, p_api[0]->baffer(player_plant[0], player_plant[1]));
    EXPECT_EQ(-1, p_api[0]->plante_sur_case(player_plant[1]).vie_max);
}

TEST_F(ApiTest, test_action_baffer_err)
{
    for (int k = 0; k < 2 * AGE_DE_POUSSE; k++)
        next_turn();

    for (int player : {0, 1})
    {
        EXPECT_EQ(HORS_POTAGER,
                  p_api[player]->baffer(position{-1, 0}, position{0, 0}));

        EXPECT_EQ(HORS_POTAGER,
                  p_api[player]->baffer(position{0, 0}, position{-1, 0}));

        EXPECT_EQ(MAUVAIS_JARDINIER,
                  p_api[player]->baffer(player_plant[1 - player],
                                        player_plant[player]));

        EXPECT_EQ(PAS_DE_PLANTE,
                  p_api[player]->baffer(position{0, 0}, player_plant[player]));

        EXPECT_EQ(PAS_DE_PLANTE,
                  p_api[player]->baffer(player_plant[player], position{0, 0}));

        EXPECT_EQ(PAS_ENCORE_ARROSEE,
                  p_api[player]->baffer(player_plant_birth[player],
                                        player_plant_birth[1 - player]));

        EXPECT_EQ(TROP_LOIN,
                  p_api[player]->baffer(player_plant[player],
                                        player_plant_too_far[player]));

        next_turn();
    }
}

TEST_F(ApiTest, test_action_baffer_self)
{
    for (int k = 0; k < 2; k++)
        for (int player : {0, 1})
        {
            EXPECT_EQ(OK, p_api[player]->baffer(player_plant[player],
                                                player_plant[player]));

            next_turn();
        }

    for (int player : {0, 1})
        EXPECT_EQ(-1,
                  p_api[player]->plante_sur_case(player_plant[player]).vie_max);
}
