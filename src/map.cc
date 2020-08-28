#include "map.hh"
#include "history.hh"
#include "utils.hh"

#include <cassert>
#include <numeric>
#include <string>

Map::Map(std::istream& stream, std::array<int, 2> player_keys)
    : player_keys(player_keys)
{
    for (size_t y = 0; y < TAILLE_GRILLE; y++)
        for (size_t x = 0; x < TAILLE_GRILLE; x++)
        {
            std::string cell_ressources_str;
            stream >> cell_ressources_str;
            const auto cell_ressources = split(cell_ressources_str, ",");

            assertm(cell_ressources.size() == 3,
                    "wrong number of ressources for cell");

            for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
            {
                ressources[x][y][k] = std::stoi(cell_ressources[k]);
                weight_plant_draining[x][y][k] = 0;
            }
        }

    for (const int player_key : player_keys)
    {
        size_t nb_plants;
        stream >> nb_plants;

        for (size_t i = 0; i < nb_plants; i++)
        {
            plante plant;

            stream >> plant.pos.x >> plant.pos.y >> plant.vie_max >>
                plant.force >> plant.elegance >> plant.rayon_deplacement >>
                plant.rayon_collecte;

            assertm(position_in_bounds(plant.pos), "plant position not valid");
            assertm(plant.vie_max > 0, "max life must be strictly positive");

            plant.consommation.resize(NB_TYPES_RESSOURCES);
            for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
                stream >> plant.consommation[k];

            plant.jardinier = player_key;
            plant.adulte = true;
            plant.enracinee = false;
            plant.age = AGE_DE_POUSSE;
            plant.vie = plant.vie_max;

            plants[plant.pos.x][plant.pos.y] = plant;
        }
    }

    plants_already_hit = init_grid(false);

    // Initialize the drainage of plants for all cells
    for (plante plant : all_plants())
        for (position cell :
             circle(plant.pos, plant.rayon_collecte / COUT_PAR_CASE_COLLECTE))
            for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
                weight_plant_draining[cell.x][cell.y][k] +=
                    plant.consommation[k];
}

void Map::new_player_turn()
{
    plants_already_hit = init_grid(false);
}

std::vector<position> Map::end_player_turn(int player_id)
{
    assert(player_id == 0 || player_id == 1);
    breed_player_plants(player_id);
    std::vector<position> deaths;

    for (auto& plant : player_plants(player_id))
    {
        ++plant.age;

        if (plant.age >= AGE_MAX)
        {
            destroy_plant(plant.pos);
            deaths.push_back(plant.pos);
        }
        else
            update_plant(plant);
    }

    return deaths;
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

std::vector<plante> Map::player_plants(int player_id) const
{
    assert(player_id == 0 || player_id == 1);
    const int key = player_key(player_id);

    return all_plants_with(
        [=](const plante& plant) { return plant.jardinier == key; });
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

Grid<bool> Map::build_has_enough_ressources() const
{
    // Check if plants have enough ressources
    Grid<bool> result = init_grid(false);
    for (plante plant : all_plants())
        result[plant.pos.x][plant.pos.y] = has_enough_ressources(plant.pos);

    return result;
}

bool Map::will_have_enough_ressources(const plante& plant)
{
    // Save current cell value
    auto current_plant = plant_at(plant.pos);

    // Check result with modified map
    plants[plant.pos.x][plant.pos.y] = plant;

    // Update the drainage in the area
    for (position cell :
         circle(plant.pos, plant.rayon_collecte / COUT_PAR_CASE_COLLECTE))
        for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
            weight_plant_draining[cell.x][cell.y][k] += plant.consommation[k];

    if (current_plant)
        for (position cell : circle(plant.pos, current_plant->rayon_collecte /
                                                   COUT_PAR_CASE_COLLECTE))
            for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
                weight_plant_draining[cell.x][cell.y][k] -=
                    current_plant->consommation[k];

    bool will_have_enough_ressources = has_enough_ressources(plant.pos);

    // Undo map modifications
    plants[plant.pos.x][plant.pos.y] = current_plant;

    for (position cell :
         circle(plant.pos, plant.rayon_collecte / COUT_PAR_CASE_COLLECTE))
        for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
            weight_plant_draining[cell.x][cell.y][k] -= plant.consommation[k];

    if (current_plant)
        for (position cell : circle(plant.pos, current_plant->rayon_collecte /
                                                   COUT_PAR_CASE_COLLECTE))
            for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
                weight_plant_draining[cell.x][cell.y][k] +=
                    current_plant->consommation[k];

    return will_have_enough_ressources;
}

bool Map::has_enough_ressources(position pos) const
{
    assert(position_in_bounds(pos));
    if (!plants[pos.x][pos.y])
        return false;

    plante plant = *plants[pos.x][pos.y];
    bool result = true;
    const auto neighbours =
        circle(plant.pos, plant.rayon_collecte / COUT_PAR_CASE_COLLECTE);

    for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
    {
        long long int divisor = 1;

        for (position cell : neighbours)
            if (weight_plant_draining[cell.x][cell.y][k] != 0)
                divisor =
                    std::lcm(divisor, weight_plant_draining[cell.x][cell.y][k]);

        long long int quotient = 0;

        for (position cell : neighbours)
            if (weight_plant_draining[cell.x][cell.y][k] != 0)
            {
                quotient +=
                    ressources[cell.x][cell.y][k] * plant.consommation[k] *
                    (divisor / weight_plant_draining[cell.x][cell.y][k]);
            }

        // We need quotient / divisor >= plant.consommation[k]
        result &= quotient >= plant.consommation[k] * divisor;
    }
    return result;
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
    for (position cell :
         circle(plants[to.x][to.y]->pos,
                plants[to.x][to.y]->rayon_collecte / COUT_PAR_CASE_COLLECTE))
        for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
            weight_plant_draining[cell.x][cell.y][k] -=
                plants[to.x][to.y]->consommation[k];

    ;
    plants[to.x][to.y]->pos = to;
    for (position cell :
         circle(plants[to.x][to.y]->pos,
                plants[to.x][to.y]->rayon_collecte / COUT_PAR_CASE_COLLECTE))
        for (size_t k = 0; k < NB_TYPES_RESSOURCES; k++)
            weight_plant_draining[cell.x][cell.y][k] +=
                plants[to.x][to.y]->consommation[k];
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

void Map::breed_player_plants(int player_id)
{
    assert(player_id == 0 || player_id == 1);
    const int key = player_key(player_id);
    const auto has_enough_ressources = build_has_enough_ressources();

    for (int x = 0; x < TAILLE_GRILLE; x++)
        for (int y = 0; y < TAILLE_GRILLE; y++)
        {
            // The cell must be empty in order to breed
            if (plant_at({x, y}).has_value())
                continue;

            // Collect adjacent plants that belong to current player, with
            // enough ressources.
            std::vector<plante> adjacents;

            for (position adj_pos : circle({x, y}, 1))
                if (adj_pos != position{x, y} &&
                    plant_at(adj_pos).has_value() &&
                    plant_at(adj_pos)->jardinier == key &&
                    has_enough_ressources[adj_pos.x][adj_pos.y])
                {
                    adjacents.push_back(*plant_at(adj_pos));
                }

            // If there is at least two plants to breed, create a child
            if (adjacents.size() >= 2)
            {
                plante new_plant = *breed(adjacents);
                new_plant.pos = {x, y};
                plants[x][y] = new_plant;

                // PlayerInfo& player_ = st->get_player_by_key(player_id_);
                internal_action action;
                action.type = birth;
                action.birth.pos = {x, y};
                // player_.add_internal_action(action);
            }
        }
}

int Map::player_key(int player_id) const
{
    assert(player_id == 0 || player_id == 1);
    return player_keys[player_id];
}
