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

            // TODO: we should not expose internal ID
            plant.jardinier = player;
            plant.adulte = true;
            // TODO: add as map argument
            plant.enracinee = false;
            plant.age = AGE_DE_POUSSE;
            plant.vie = plant.vie_max;

            plants[plant.pos.x][plant.pos.y] = plant;
        }
    }
}

void Map::new_player_turn()
{
    plants_already_hit = init_grid(false);
}

void Map::end_player_turn(int player_key)
{
    for (auto& plant : player_plants(player_key))
    {
        ++plant.age;
        update_plant(plant);
        if (plant.age >= AGE_MAX)
        {
            destroy_plant(plant.pos);
            // TODO add death internal_action
        }
    }
}

std::array<int, NB_TYPES_RESSOURCES> Map::ressources_at(position pos) const
{
    assert(position_in_bounds(pos));
    return ressources[pos.x][pos.y];
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

void Map::update_plant(const plante& plant)
{
    assert(position_in_bounds(plant.pos));
    assert(plants[plant.pos.x][plant.pos.y]);
    plants[plant.pos.x][plant.pos.y] = plant;
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

    Grid<bool> result = init_grid(false);

    for (plante plant : all_plants())
    {
        result[plant.pos.x][plant.pos.y] = true;
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

bool Map::has_enough_ressources(position pos) const
{
    assert(position_in_bounds(pos));
    return build_has_enough_ressources()[pos.x][pos.y];
}

bool Map::already_hit(position pos) const
{
    assert(position_in_bounds(pos));
    return plants_already_hit[pos.x][pos.y];
}

void Map::move_plant(position from, position to)
{
    assert(position_in_bounds(from));
    assert(position_in_bounds(to));
    assert(plants[from.x][from.y]);
    assert(!plants[to.x][to.y]);
    plants[from.x][from.y].swap(plants[to.x][to.y]);
    plants[to.x][to.y]->pos = to;
    plants[to.x][to.y]->enracinee = true;
}

bool Map::hit(position attacker_pos, position victim_pos)
{
    assert(plant_at(attacker_pos).has_value());
    assert(plant_at(victim_pos).has_value());
    assert(!already_hit(attacker_pos));

    plante& attacker = *plants[attacker_pos.x][attacker_pos.y];
    plante& victim = *plants[victim_pos.x][victim_pos.y];

    plants_already_hit[attacker.pos.x][attacker.pos.y] = true;
    victim.vie -= attacker.force;

    if (victim.vie <= 0)
    {
        destroy_plant(victim.pos);
        return true;
    }

    return false;
}

void Map::destroy_plant(position pos)
{
    assert(position_in_bounds(pos));
    plants[pos.x][pos.y].reset();
}
