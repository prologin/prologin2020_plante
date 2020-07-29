// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <array>
#include <cmath>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include "constant.hh"

// Assertion with message
#define assertm(exp, msg) assert(((void)msg, exp))

// Statistics of a plant for invalid outputs
const plante INVALID_PLANT = {{-1, -1}, -1, false, false, -1, -1,
                              -1,       -1, -1,    -1,    {}, -1};

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

// Check if a plant is allowed to jump to a given position.
bool plant_can_jump(const plante& plant, position dest);

// Return rounded result of x / q.
int round_div(int x, int q);

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

// ---
// --- Plants operators
// ---

inline bool operator==(const plante& p1, const plante& p2)
{
    return p1.pos == p2.pos && p1.jardinier == p2.jardinier &&
           p1.adulte == p2.adulte && p1.enracinee == p2.enracinee &&
           p1.vie == p2.vie && p1.vie_max == p2.vie_max &&
           p1.force == p2.force && p1.elegance == p2.elegance &&
           p1.rayon_deplacement == p2.rayon_deplacement &&
           p1.rayon_collecte == p2.rayon_collecte &&
           p1.consommation == p2.consommation && p1.age == p2.age;
}

void to_json(std::ostream& o, const position& pos);
