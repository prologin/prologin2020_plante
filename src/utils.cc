// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <cassert>
#include <functional>
#include <unordered_set>

#include "utils.hh"

std::vector<std::string> split(std::string text, const std::string& delim)
{
    std::vector<std::string> parts;
    size_t pos = 0;
    std::string token;

    while ((pos = text.find(delim)) != std::string::npos)
    {
        parts.push_back(text.substr(0, pos));
        text.erase(0, pos + delim.length());
    }

    parts.push_back(text);
    return parts;
}

bool position_in_bounds(const position& position)
{
    return position.x >= 0 && position.y >= 0 && position.x < TAILLE_GRILLE &&
           position.y < TAILLE_GRILLE;
}

int distance(position pos1, position pos2)
{
    return std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y);
}

std::vector<position> circle(position center, int radius)
{
    if (radius < 0 || !position_in_bounds(center))
        return {};

    Grid<bool> visited = init_grid(false);
    visited[center.x][center.y] = true;

    std::vector<position> circle = {center};
    size_t cursor = 0;

    while (cursor < circle.size())
    {
        position curr = circle[cursor];

        std::array<position, 4> neighbours;
        neighbours[0] = {curr.x + 1, curr.y};
        neighbours[1] = {curr.x - 1, curr.y};
        neighbours[2] = {curr.x, curr.y + 1};
        neighbours[3] = {curr.x, curr.y - 1};

        for (position neighbour : neighbours)
            if (position_in_bounds(neighbour) &&
                distance(center, neighbour) <= radius &&
                !visited[neighbour.x][neighbour.y])
            {
                visited[neighbour.x][neighbour.y] = true;
                circle.push_back(neighbour);
            }

        cursor++;
    }

    return circle;
}

bool plant_can_jump(const plante& plant, position dest)
{
    const int dist = distance(plant.pos, dest);
    const int max_dist = plant.rayon_deplacement / COUT_PAR_CASE_DEPOTAGE;
    return dist <= max_dist;
}

int round_div(int x, int q)
{
    int result = x / q;

    // Round up when (x % q) / q >= 1/2
    if (2 * (x % q) >= q)
        result += 1;

    return result;
}

std::optional<plante> breed(const std::vector<plante>& parents)
{
    if (parents.size() <= 1)
        return {};

    // Check that all plants belong two the same player
    assert(std::all_of(parents.begin() + 1, parents.end(), [&](auto& plant) {
        return plant.jardinier == parents[0].jardinier;
    }));

    const auto mean_for = [&](std::function<int(const plante&)> val) -> int {
        int value = 0;

        for (const plante& plant : parents)
            value += val(plant);

        return round_div(value, parents.size());
    };

    std::vector<int> consommation(NB_TYPES_RESSOURCES);

    for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
        consommation[k] = mean_for([=](auto p) { return p.consommation[k]; });

    return {{{-1, -1},
             parents[0].jardinier,
             false,
             false,
             mean_for([](auto p) { return p.vie_max; }),
             mean_for([](auto p) { return p.vie_max; }),
             mean_for([](auto p) { return p.force; }),
             mean_for([](auto p) { return p.elegance; }),
             mean_for([](auto p) { return p.rayon_deplacement; }),
             mean_for([](auto p) { return p.rayon_collecte; }),
             consommation,
             0}};
}

void to_json(std::ostream& o, const position& pos)
{
    o << "{\"x\": " << pos.x << ", \"y\": " << pos.y << "}";
}
