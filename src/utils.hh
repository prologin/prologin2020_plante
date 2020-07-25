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

std::vector<std::string> split(std::string text, const std::string& delim);

bool position_in_bounds(const position& position);

std::vector<std::string> split(std::string text, const std::string& delim);

int distance(position pos1, position pos2);

std::vector<position> circle(position center, int radius);
