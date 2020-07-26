// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <array>
#include <cmath>
#include <optional>
#include <string>
#include <vector>

#include "constant.hh"

// Assertion with message
#define assertm(exp, msg) assert(((void)msg, exp))

// Any information attached to all cells of the map
template <typename T>
using Grid = std::array<std::array<T, TAILLE_GRILLE>, TAILLE_GRILLE>;

// Initialize a grid filled with a single value
template <typename T>
Grid<T> init_grid(T default_value)
{
    Grid<T> result;

    for (auto& row : result)
        for (T& item : row)
            item = default_value;

    return result;
}

// Split a string a each occurence of given separator
std::vector<std::string> split(std::string text, const std::string& delim);

// Check if a position represents a cell of the grid
bool position_in_bounds(const position& position);

// Compute distance between two positions
int distance(position pos1, position pos2);

// Return the list of cells in the grid that are at distance at most `radius`
// from `center`. If the center is out of the grid, the output is empty.
std::vector<position> circle(position center, int radius);

// Return rounded result of x / q.
int round_div(int x, int q);

// Statistics of a plant for invalid outputs
plante invalid_plant();

// Get the statistics of a plant breeded by a set of parents.
std::optional<plante> breed(const std::vector<plante>& parents);

// ---
// --- Position operators
// ---

inline bool operator==(const position& pos1, const position& pos2)
{
    return pos1.x == pos2.x && pos1.y == pos2.y;
}

inline bool operator!=(const position& pos1, const position& pos2)
{
    return !(pos1 == pos2);
}
