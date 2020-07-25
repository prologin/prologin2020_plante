// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <array>
#include <istream>
#include <optional>

#include "constant.hh"

template <typename T>
using Grid = std::array<std::array<T, TAILLE_GRILLE>, TAILLE_GRILLE>;

class Map
{
public:
    Map(std::istream& stream);

private:
    Grid<std::array<int, NB_TYPES_RESSOURCES>> ressources;
    Grid<std::optional<plante>> plants;
};
