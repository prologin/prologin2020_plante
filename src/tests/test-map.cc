// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <gtest/gtest.h>

#include "../map.hh"
#include "test-helpers.hh"

#include <iostream>
#include <sstream>

TEST(MapTest, Deserialize)
{
    std::istringstream map_buff(test_map);
    const Map map(map_buff, {42, 1337});
    const plante plant = *map.plant_at({2, 2});

    EXPECT_EQ(plant.pos.x, 2);
    EXPECT_EQ(plant.pos.y, 2);
    EXPECT_EQ(plant.vie_max, 110);
    EXPECT_EQ(plant.force, 60);
    EXPECT_EQ(plant.elegance, 42);
    EXPECT_EQ(plant.rayon_deplacement, 8);
    EXPECT_EQ(plant.rayon_collecte, 75);
    EXPECT_EQ(plant.jardinier, 1337);
    EXPECT_EQ(plant.adulte, true);
    EXPECT_EQ(plant.enracinee, false);
    EXPECT_EQ(plant.age, AGE_DE_POUSSE);
    EXPECT_EQ(plant.vie, 110);
}

TEST(MapTest, drainage_test) {
    std::istringstream map_buff(test_map);
    Map map(map_buff, {42, 1337});
    plante plant = *map.plant_at({2, 2});
    EXPECT_EQ(map.has_enough_ressources(plant.pos),true);
    EXPECT_EQ(map.will_have_enough_ressources(plant),true);
    EXPECT_EQ(map.has_enough_ressources(plant.pos),true);

    plant.pos = {0, 0};
    plant.rayon_collecte = 7 * COUT_PAR_CASE_COLLECTE;
    plant.consommation = {8, 1, 4};
    EXPECT_EQ(map.will_have_enough_ressources(plant),false);

}
