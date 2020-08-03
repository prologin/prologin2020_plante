// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <map>

#include <gtest/gtest.h>

#include "test-helpers.hh"

// Test API getters, see specific test files for actions

TEST_F(ApiTest, test_debug_afficher_chien)
{
    for (auto api : p_api)
    {
        EXPECT_EQ(OK, api->debug_afficher_chien(position{0, 0}, CHIEN_BLEU));
        EXPECT_EQ(OK, api->debug_afficher_chien(position{0, 0}, CHIEN_ROUGE));
    }
}

TEST_F(ApiTest, test_plantes_jardinier)
{
    std::map<int, int> nb_plants = {{1337, 5}, {42, 6}};

    for (auto api : p_api)
    {
        EXPECT_TRUE(api->plantes_jardinier(0).empty());

        for (int player : {1337, 42})
            EXPECT_EQ(nb_plants[player], api->plantes_jardinier(player).size());
    }
}

TEST_F(ApiTest, test_plante_sur_case)
{
    for (auto api : p_api)
    {
        EXPECT_EQ(42, api->plante_sur_case({2, 2}).jardinier);
        EXPECT_EQ(-1, api->plante_sur_case({0, 0}).jardinier);
        EXPECT_EQ(-1, api->plante_sur_case({-1, -1}).jardinier);
    }
}

TEST_F(ApiTest, test_plantes_arrosables)
{
    next_turn();
    next_turn();

    for (auto api : p_api)
        for (int player : {1337, 42})
            EXPECT_TRUE(api->plantes_arrosables(player).empty());

    for (int k = 0; k < 2 * (AGE_DE_POUSSE - 1); k++)
        next_turn();

    for (auto api : p_api)
        EXPECT_TRUE(api->plantes_arrosables(0).empty());

    std::map<int, int> nb_plants = {{1337, 2}, {42, 1}};

    for (auto api : p_api)
        for (int player : {1337, 42})
            EXPECT_EQ(nb_plants[player],
                      api->plantes_arrosables(player).size());
}

TEST_F(ApiTest, test_plantes_adultes)
{
    std::map<int, int> nb_plants = {{1337, 5}, {42, 6}};

    for (int k = 0; k < 2 * AGE_DE_POUSSE; k++)
        next_turn();

    for (auto api : p_api)
        EXPECT_TRUE(api->plantes_adultes(0).empty());

    for (auto api : p_api)
        for (int player : {1337, 42})
            EXPECT_EQ(nb_plants[player], api->plantes_adultes(player).size());
}

TEST_F(ApiTest, test_plantes_depotables)
{
    std::map<int, int> nb_plants = {{1337, 5}, {42, 6}};

    for (int k = 0; k < 2 * AGE_DE_POUSSE; k++)
        next_turn();

    for (auto api : p_api)
    {
        EXPECT_EQ(nb_plants[api->moi()],
                  api->plantes_depotables(api->moi()).size());

        for (auto plant : api->plantes_arrosables(api->moi()))
            EXPECT_EQ(OK, api->arroser(plant.pos, CARACTERISTIQUE_ELEGANCE));

        EXPECT_EQ(7, api->plantes_depotables(api->moi()).size());
    }
}

TEST_F(ApiTest, test_ressources_sur_case)
{
    for (auto api : p_api)
    {
        EXPECT_EQ((std::vector<int>{0, 0, 0}),
                  api->ressources_sur_case({0, 0}));

        EXPECT_EQ((std::vector<int>{8, 0, 0}),
                  api->ressources_sur_case({6, 3}));

        EXPECT_EQ((std::vector<int>{8, 0, 3}),
                  api->ressources_sur_case({8, 4}));

        EXPECT_EQ((std::vector<int>{0, 0, 0}),
                  api->ressources_sur_case({-1, -1}));
    }
}

TEST_F(ApiTest, test_reproduction_possible)
{
    // TODO
}

TEST_F(ApiTest, test_plante_reproductible)
{
    // TODO
}

TEST_F(ApiTest, test_croisement)
{
    const plante plant_1 = {{-1, -1}, -1, false, false, 100,       100,
                            50,       5,  2,     3,     {0, 0, 0}, 0};

    const plante plant_2 = {{-1, -1}, -1, true, true, 100,        200,
                            50,       5,  2,    3,    {0, 5, 10}, 0};

    const plante plant_3 = {{-1, -1}, -1, true, true, 100,        100,
                            50,       5,  2,    3,    {0, 5, 10}, 0};

    for (auto api : p_api)
    {
        EXPECT_EQ(-1, api->croisement({plant_1}).vie_max);
        EXPECT_EQ(plant_1, api->croisement({plant_1, plant_1, plant_1}));
        EXPECT_EQ(150, api->croisement({plant_1, plant_2}).vie_max);
        EXPECT_EQ(133, api->croisement({plant_1, plant_2, plant_3}).vie_max);
    }
}

TEST_F(ApiTest, test_historique)
{
    // TODO
}

TEST_F(ApiTest, test_score)
{
    std::map<int, int> per_turn_elegance = {{1337, 2}, {42, 46}};

    for (int turn = 0; turn <= 6; turn++)
    {
        for (auto api : p_api)
            for (int jardinier : {42, 1337})
                EXPECT_EQ(turn * per_turn_elegance[jardinier],
                          api->score(jardinier));

        next_turn();
        next_turn();
    }
}

TEST_F(ApiTest, test_moi)
{
    for (auto api : p_api)
        EXPECT_EQ((int)api->player()->id, api->moi());
}

TEST_F(ApiTest, test_adversaire)
{
    for (size_t p_id : {0, 1})
    {
        auto my_api = p_api[p_id];
        auto op_api = p_api[1 - p_id];
        EXPECT_EQ(my_api->adversaire(), op_api->moi());
    }
}
