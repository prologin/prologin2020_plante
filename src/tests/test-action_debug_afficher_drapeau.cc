// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <gtest/gtest.h>

#include "test-helpers.hh"

TEST_F(ApiTest, test_debug_afficher_chien)
{
    for (auto api : p_api)
    {
        for (auto color : {AUCUN_CHIEN, CHIEN_BLEU, CHIEN_ROUGE, CHIEN_VERT})
        {
            EXPECT_EQ(OK, api->debug_afficher_chien(position{4, 5}, color));
            EXPECT_EQ(HORS_POTAGER,
                      api->debug_afficher_chien(position{-1, 5}, color));
        }

        EXPECT_EQ(CHIEN_INVALIDE,
                  api->debug_afficher_chien(position{4, 5}, (debug_chien)-1));

        EXPECT_EQ(CHIEN_INVALIDE,
                  api->debug_afficher_chien(position{4, 5}, (debug_chien)4));
    }
}
