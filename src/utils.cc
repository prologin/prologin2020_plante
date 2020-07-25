// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

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
    if (radius < 0)
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
