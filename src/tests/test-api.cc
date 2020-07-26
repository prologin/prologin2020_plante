#include <gtest/gtest.h>

#include "test-helpers.hh"

// Test API getters, see specific test files for actions

TEST_F(ApiTest, test_debug_afficher_chien)
{
    // TODO
}

TEST_F(ApiTest, test_plantes_jardinier)
{
    // TODO
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
    // TODO
}

TEST_F(ApiTest, test_plantes_adultes)
{
    // TODO
}

TEST_F(ApiTest, test_plantes_depotables)
{
    // TODO
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
    // TODO
}

TEST_F(ApiTest, test_moi)
{
    for (auto api : p_api)
        EXPECT_EQ(api->player()->id, api->moi());
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
