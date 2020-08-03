// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <gtest/gtest.h>

#include "test-helpers.hh"

const std::vector<position> player_plant = {{17, 18}, {18, 19}};
const std::vector<position> player_dest = {{16, 17}, {16, 19}};
const std::vector<position> player_far = {{16, 16}, {15, 19}};
const std::vector<position> player_birth = {{17, 17}, {19, 19}};

TEST_F(ApiTest, test_depoter_ok)
{
    for (int player : {0, 1})
    {
        EXPECT_EQ(OK, p_api[player]->depoter(player_plant[player],
                                             player_dest[player]));

        EXPECT_EQ(SANS_POT, p_api[player]->depoter(player_dest[player],
                                                   player_plant[player]));

        EXPECT_EQ(0, p_api[player]
                         ->plante_sur_case(player_dest[player])
                         .consommation[2]);

        next_turn();
    }
}

TEST_F(ApiTest, test_depoter_err)
{
    for (int k = 0; k < 2; k++)
        next_turn();

    for (int player : {0, 1})
    {
        EXPECT_EQ(HORS_POTAGER,
                  p_api[player]->depoter(position{-1, 0}, position{0, 0}));

        EXPECT_EQ(HORS_POTAGER, p_api[player]->depoter(player_plant[player],
                                                       position{-1, 0}));

        EXPECT_EQ(PAS_DE_PLANTE,
                  p_api[player]->depoter(position{0, 0}, position{1, 1}));

        EXPECT_EQ(MAUVAIS_JARDINIER,
                  p_api[player]->depoter(player_plant[1 - player],
                                         player_dest[1 - player]));

        EXPECT_EQ(CASE_OCCUPEE,
                  p_api[player]->depoter(player_plant[player],
                                         player_plant[1 - player]));

        EXPECT_EQ(CASE_OCCUPEE, p_api[player]->depoter(player_plant[player],
                                                       player_plant[player]));

        EXPECT_EQ(TROP_LOIN, p_api[player]->depoter(player_plant[player],
                                                    player_far[player]));

        EXPECT_EQ(
            PAS_ENCORE_ARROSEE,
            p_api[player]->depoter(player_birth[player], player_dest[player]));

        next_turn();
    }
}
