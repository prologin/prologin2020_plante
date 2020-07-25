// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <array>
#include <cmath>
#include <string>
#include <vector>

#include "constant.hh"

template <typename T>
using Grid = std::array<std::array<T, TAILLE_GRILLE>, TAILLE_GRILLE>;

template <typename T>
Grid<T> init_grid(T default_value)
{
    Grid<T> result;

    for (auto& row : result)
        for (T& item : row)
            item = default_value;

    return result;
}

std::vector<std::string> split(std::string text, const std::string& delim);

bool position_in_bounds(const position& position);

std::vector<std::string> split(std::string text, const std::string& delim);

int distance(position pos1, position pos2);

std::vector<position> circle(position center, int radius);
