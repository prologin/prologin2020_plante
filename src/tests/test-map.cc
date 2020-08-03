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
