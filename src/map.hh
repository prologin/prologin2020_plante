// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <istream>
#include <optional>

#include "constant.hh"
#include "utils.hh"

class Map
{
public:
    Map(std::istream& stream);
    std::optional<plante> plant_at(position pos) const;
    void move_plant(position from, position to);
    void destroy_plant(position pos);

private:
    Grid<std::array<int, NB_TYPES_RESSOURCES>> ressources;
    Grid<std::optional<plante>> plants;
};
