// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <gtest/gtest.h>

#include "../utils.hh"
#include "test-helpers.hh"

TEST(UtilsTest, init_grid)
{
    Grid<int> grid = init_grid(42);

    for (size_t x = 0; x < TAILLE_GRILLE; x++)
        for (size_t y = 0; y < TAILLE_GRILLE; y++)
            EXPECT_EQ(grid[x][y], 42);
}

TEST(UtilsTest, split)
{
    std::string text = "Hello, how are you?";
    std::vector<std::string> expected = {"Hello,", "how", "are", "you?"};
    EXPECT_EQ(split(text, " "), expected);
}

TEST(UtilsTest, position_in_bounds)
{
    EXPECT_TRUE(position_in_bounds({TAILLE_GRILLE - 1, TAILLE_GRILLE - 1}));
    EXPECT_TRUE(position_in_bounds({0, 0}));
    EXPECT_TRUE(position_in_bounds({0, TAILLE_GRILLE - 1}));
    EXPECT_FALSE(position_in_bounds({-1, 0}));
    EXPECT_FALSE(position_in_bounds({TAILLE_GRILLE + 13, 1}));
    EXPECT_FALSE(position_in_bounds({TAILLE_GRILLE, TAILLE_GRILLE}));
}

TEST(UtilsTest, distance)
{
    EXPECT_EQ(distance({42, 1337}, {42, 1337}), 0);
    EXPECT_EQ(distance({-1, 1}, {1, -1}), 4);
    EXPECT_EQ(distance({1, -3}, {3, -10}), 9);
}

TEST(UtilsTest, circle)
{
    EXPECT_TRUE(same_sets(circle({5, 5}, 0), {{5, 5}}));
    EXPECT_TRUE(same_sets(circle({5, 5}, -1), {}));
    EXPECT_TRUE(same_sets(circle({-5, -5}, 10), {}));

    EXPECT_TRUE(same_sets(circle({0, 0}, 3), {{0, 0},
                                              {1, 0},
                                              {2, 0},
                                              {3, 0},
                                              {0, 1},
                                              {1, 1},
                                              {2, 1},
                                              {0, 2},
                                              {1, 2},
                                              {0, 3}}));

    EXPECT_TRUE(same_sets(circle({10, 10}, 1),
                          {{10, 10}, {9, 10}, {10, 9}, {11, 10}, {10, 11}}));
}
