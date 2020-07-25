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
    // TODO
    abort();
}
int Api::score(int id_joueur)
{
    // TODO
    abort();
}
int Api::moi()
{
    return player_->id;
}
int Api::adversaire()
{
    // TODO
    abort();
}
bool Api::annuler()
{
    // TODO
    abort();
}
int Api::tour_actuel()
{
    return game_state_->get_round();
}
