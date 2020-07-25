// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"

#include <algorithm>
#include <memory>
#include <utility>

inline bool player_valid(int player_id)
{
    // TODO check if == moi() || adversaire()
    return player_id != -1;
}

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
    // TODO
    abort();
}

plante Api::plante_sur_case(position pos)
{
    return *game_state_->get_map().plant_at(pos);
}

std::vector<plante> Api::plantes_arrosables(int joueur)
{
    // TODO
    abort();
}

std::vector<plante> Api::plantes_adultes(int joueur)
{
    // TODO
    abort();
}

std::vector<plante> Api::plantes_depotables(int joueur)
{
    // TODO
    abort();
}

std::vector<int> Api::ressources_sur_case(position pos)
{
    // TODO
    abort();
}

bool Api::reproduction_possible(position pos, int rayon_collecte,
                                std::vector<int> consommation)
{
    // TODO
    abort();
}

bool Api::plante_reproductible(position pos)
{
    // TODO
    abort();
}

plante Api::croisement(std::vector<plante> parents)
{
    // TODO
    abort();
}

std::vector<action_hist> Api::historique()
{
    int opponent_id = game_state_->get_player_key_by_id(adversaire());
    return game_state_->get_history(opponent_id);
}

int Api::score(int id_joueur)
{
    if (!player_valid(id_joueur))
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
