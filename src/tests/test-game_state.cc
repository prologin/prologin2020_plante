// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <gtest/gtest.h>

#include "test-helpers.hh"

TEST_F(ApiTest, test_breeding)
{
    for (auto api : p_api)
    {
        EXPECT_EQ(-1, api->plante_sur_case({18, 18}).jardinier);
        EXPECT_EQ(-1, api->plante_sur_case({19, 19}).jardinier);
    }

    // End of turn / breeding for player 1
    next_turn();

    for (auto api : p_api)
    {
        EXPECT_EQ(1337, api->plante_sur_case({18, 18}).jardinier);
        EXPECT_EQ(-1, api->plante_sur_case({19, 19}).jardinier);
    }

    // End of turn / breeding for player 2
    next_turn();

    for (auto api : p_api)
    {
        EXPECT_EQ(1337, api->plante_sur_case({18, 18}).jardinier);
        EXPECT_EQ(42, api->plante_sur_case({19, 19}).jardinier);
    }
}

TEST_F(ApiTest, test_breeding_stats)
{
    next_turn();

    for (auto api : p_api)
    {
        EXPECT_EQ(111, api->plante_sur_case({18, 18}).vie_max);
        EXPECT_EQ(0, api->plante_sur_case({18, 18}).consommation[0]);
        EXPECT_EQ(112, api->plante_sur_case({18, 18}).consommation[1]);
    }
}

TEST_F(ApiTest, test_breeding_condition)
{
    // TODO
}

TEST_F(ApiTest, test_aging)
{
    for (auto api : p_api)
    {
        EXPECT_EQ(AGE_DE_POUSSE, api->plante_sur_case({18, 19}).age);
        EXPECT_EQ(-1, api->plante_sur_case({18, 18}).age);
        EXPECT_EQ(-1, api->plante_sur_case({19, 19}).age);
    }

    next_turn();

    for (auto api : p_api)
    {
        EXPECT_EQ(AGE_DE_POUSSE, api->plante_sur_case({18, 19}).age);
        EXPECT_EQ(1, api->plante_sur_case({18, 18}).age);
        EXPECT_EQ(-1, api->plante_sur_case({19, 19}).age);
    }

    next_turn();

    for (auto api : p_api)
    {
        EXPECT_EQ(1 + AGE_DE_POUSSE, api->plante_sur_case({18, 19}).age);
        EXPECT_EQ(1, api->plante_sur_case({18, 18}).age);
        EXPECT_EQ(1, api->plante_sur_case({19, 19}).age);
    }

    for (int k = 1 + AGE_DE_POUSSE; k + 1 < AGE_MAX; k++)
    {
        next_turn();
        next_turn();
    }

    for (auto api : p_api)
        EXPECT_EQ(AGE_MAX - 1, api->plante_sur_case({18, 19}).age);

    next_turn();
    next_turn();

    for (auto api : p_api)
        EXPECT_EQ(-1, api->plante_sur_case({18, 19}).age);
}

TEST_F(ApiTest, test_score)
{
    // TODO
}
