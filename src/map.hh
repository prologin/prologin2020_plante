// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <functional>
#include <istream>
#include <optional>

#include "constant.hh"
#include "utils.hh"

class Map
{
public:
    Map(std::istream& stream);

    std::vector<plante>
    all_plants_with(std::function<bool(const plante&)> predicate) const;
    std::vector<plante> all_plants() const;
    std::vector<plante> player_plants(int player) const;

    std::optional<plante> plant_at(position pos) const;
    Grid<bool> build_has_enough_ressources() const;

    void move_plant(position from, position to);
    void destroy_plant(position pos);

private:
    Grid<std::array<int, NB_TYPES_RESSOURCES>> ressources;
    Grid<std::optional<plante>> plants;
};
