#include "map.hh"
#include "utils.hh"

#include <cassert>
#include <string>

Map::Map(std::istream& stream)
{
    for (size_t x = 0; x < TAILLE_GRILLE; x++)
        for (size_t y = 0; y < TAILLE_GRILLE; y++)
        {
            std::string cell_ressources_str;
            stream >> cell_ressources_str;
            const auto cell_ressources = split(cell_ressources_str, ",");

            for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
                ressources[x][y][k] = std::stoi(cell_ressources[k]);
        }

    for (const int player : {0, 1})
    {
        size_t nb_plants;
        stream >> nb_plants;

        for (size_t i = 0; i < nb_plants; i++)
        {
            plante plant;

            stream >> plant.pos.x >> plant.pos.y >> plant.vie_max >>
                plant.force >> plant.elegance >> plant.rayon_deplacement >>
                plant.rayon_collecte;

            for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
                stream >> plant.consommation[k];

            plant.jardinier = player;
            plant.adulte = true;
            plant.enracinee = false;
            plant.age = AGE_DE_POUSSE;
            plant.vie = plant.vie_max;

            plants[plant.pos.x][plant.pos.y] = plant;
        }
    }
}

std::vector<plante>
Map::all_plants_with(std::function<bool(const plante&)> predicate) const
{
    std::vector<plante> result;

    for (const auto& row : plants)
        for (const auto& cell : row)
            if (const auto plant = cell)
                if (predicate(*plant))
                    result.push_back(*plant);

    return result;
}

std::vector<plante> Map::all_plants() const
{
    return all_plants_with([](auto) { return true; });
}

std::vector<plante> Map::player_plants(int player) const
{
    return all_plants_with(
        [player](const plante& plant) { return plant.jardinier == player; });
}

std::optional<plante> Map::plant_at(position pos) const
{
    assert(position_in_bounds(pos));
    return plants[pos.x][pos.y];
}

// TODO: cache result
Grid<bool> Map::build_has_enough_ressources() const
{
    // Compute cumulated weight of close plants for all cells of the grid

    Grid<std::array<int, NB_TYPES_RESSOURCES>> collect_total_weight =
        init_grid(std::array<int, NB_TYPES_RESSOURCES>({0, 0, 0}));

    for (plante plant : all_plants())
        for (position cell : circle(plant.pos, plant.rayon_collecte))
            for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
                collect_total_weight[cell.x][cell.y][k] +=
                    plant.consommation[k];

    // Check if plants have enough ressources

    Grid<bool> result = init_grid(true);

    for (plante plant : all_plants())
    {
        const auto neighbours = circle(plant.pos, plant.rayon_deplacement);

        for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
        {
            int quotient = 0;
            int divisor = 0;

            for (position cell : neighbours)
            {
                divisor += collect_total_weight[cell.x][cell.y][k];
                quotient +=
                    ressources[cell.x][cell.y][k] * plant.consommation[k];
            }

            // We need quotient / divisor <= plant.consommation[k]
            result[plant.pos.x][plant.pos.y] &=
                quotient <= plant.consommation[k] * divisor;
        }
    }

    return result;
}

void Map::move_plant(position from, position to)
{
    assert(position_in_bounds(from));
    assert(position_in_bounds(to));
    assert(plants[from.x][from.y]);
    assert(!plants[to.x][to.y]);
    plants[from.x][from.y].swap(plants[to.x][to.y]);
}

void Map::destroy_plant(position pos)
{
    assert(position_in_bounds(pos));
    plants[pos.x][pos.y].reset();
}
