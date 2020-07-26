// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"

#include <algorithm>
#include <memory>
#include <utility>

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, erreur>(std::move(game_state), player)
{
    api = this;
}

std::vector<plante> Api::plantes_jardinier(int jardinier)
{
    if (!game_state_->player_valid(jardinier))
        return {};

    int player_id = game_state().get_player_id_by_key(jardinier);
    return game_state().get_map().player_plants(player_id);
}

plante Api::plante_sur_case(position pos)
{
    if (!position_in_bounds(pos))
        return INVALID_PLANT;

    return game_state_->get_map().plant_at(pos).value_or(INVALID_PLANT);
}

// TODO: joueur should be called jardinier
std::vector<plante> Api::plantes_arrosables(int joueur)
{
    if (!game_state_->player_valid(joueur))
        return {};

    int player_id = game_state().get_player_id_by_key(joueur);

    const auto filter = [=](const plante& plant) {
        return plant.jardinier == player_id && plant.age >= AGE_DE_POUSSE;
    };

    return game_state().get_map().all_plants_with(filter);
}

// TODO: joueur should be called jardinier
std::vector<plante> Api::plantes_adultes(int joueur)
{
    if (!game_state_->player_valid(joueur))
        return {};

    int player_id = game_state().get_player_id_by_key(joueur);

    const auto filter = [=](const plante& plant) {
        return plant.jardinier == player_id && plant.adulte;
    };

    return game_state().get_map().all_plants_with(filter);
}

// TODO: joueur should be called jardinier
std::vector<plante> Api::plantes_depotables(int joueur)
{
    if (!game_state_->player_valid(joueur))
        return {};

    int player_id = game_state().get_player_id_by_key(joueur);

    const auto filter = [=](const plante& plant) {
        return plant.jardinier == player_id && !plant.enracinee;
    };

    return game_state().get_map().all_plants_with(filter);
}

std::vector<int> Api::ressources_sur_case(position pos)
{
    if (!position_in_bounds(pos))
        return {0, 0, 0};

    const auto ressources = game_state().get_map().ressources_at(pos);
    return std::vector(ressources.begin(), ressources.end());
}

bool Api::reproduction_possible(position pos, int rayon_collecte,
                                std::vector<int> consommation)
{
    // TODO
    abort();
}

bool Api::plante_reproductible(position pos)
{
    if (!position_in_bounds(pos) ||
        !game_state().get_map().plant_at(pos).has_value())
        return false;

    return game_state().get_map().has_enough_ressources(pos);
}

plante Api::croisement(std::vector<plante> parents)
{
    return breed(parents).value_or(INVALID_PLANT);
}

std::vector<action_hist> Api::historique()
{
    int opponent_id = game_state_->get_player_key_by_id(adversaire());
    return game_state_->get_history(opponent_id);
}

int Api::score(int id_joueur)
{
    if (!game_state_->player_valid(id_joueur))
        return -1;

    return game_state_->get_player_by_key(id_joueur).get_score();
}

int Api::moi()
{
    return player_->id;
}

int Api::adversaire()
{
    const int player_id = game_state_->get_player_id_by_key(moi());
    const int opponent_id = game_state_->get_opponent_id(player_id);
    return game_state_->get_player_key_by_id(opponent_id);
}

int Api::tour_actuel()
{
    return game_state_->get_round();
}
