// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright 2020 Prologin <info@prologin.org>

#include <gtest/gtest.h>

#include "test-helpers.hh"

std::vector<position> player_plant = {{17, 17}, {19, 19}};

TEST_F(ApiTest, test_action_arroser_ok)
{
    for (int k = 0; k < 2 * AGE_DE_POUSSE; k++)
        next_turn();

    for (int player : {0, 1})
    {
        const int elegance_bfor =
            p_api[player]->plante_sur_case(player_plant[player]).elegance;

        EXPECT_EQ(OK, p_api[player]->arroser(player_plant[player],
                                             CARACTERISTIQUE_ELEGANCE));

        EXPECT_EQ(
            elegance_bfor + APPORT_CARACTERISTIQUE,
            p_api[player]->plante_sur_case(player_plant[player]).elegance);

        EXPECT_EQ(DEJA_ARROSEE,
                  p_api[player]->arroser(player_plant[player],
                                         CARACTERISTIQUE_ELEGANCE));

        next_turn();
    }

    for (int player : {0, 1})
    {
        EXPECT_EQ(DEJA_ARROSEE,
                  p_api[player]->arroser(player_plant[player],
                                         CARACTERISTIQUE_ELEGANCE));

        next_turn();
    }
}

TEST_F(ApiTest, test_action_arroser_too_early)
{
    for (int k = 0; k < 2 * (AGE_DE_POUSSE - 1); k++)
        next_turn();

    for (int player : {0, 1})
    {
        EXPECT_EQ(PAS_ENCORE_ADULTE,
                  p_api[player]->arroser(player_plant[player],
                                         CARACTERISTIQUE_ELEGANCE));
        next_turn();
    }
}

TEST_F(ApiTest, test_action_arroser_wrong_player)
{
    for (int k = 0; k < 2 * AGE_DE_POUSSE; k++)
        next_turn();

    for (int player : {0, 1})
    {
        EXPECT_EQ(MAUVAIS_JARDINIER,
                  p_api[player]->arroser(player_plant[1 - player],
                                         CARACTERISTIQUE_ELEGANCE));

        next_turn();
    }
}

TEST_F(ApiTest, test_action_arroser_hors_map)
{
    for (int k = 0; k < 2 * AGE_DE_POUSSE; k++)
        next_turn();

    for (int player : {0, 1})
    {
        EXPECT_EQ(HORS_POTAGER, p_api[player]->arroser(
                                    position{-1, 4}, CARACTERISTIQUE_ELEGANCE));

        EXPECT_EQ(PAS_DE_PLANTE, p_api[player]->arroser(
                                     position{0, 0}, CARACTERISTIQUE_ELEGANCE));
    }
}
