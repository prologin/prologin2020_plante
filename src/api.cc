// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "api.hh"

#include <memory>
#include <utility>

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state,
         std::shared_ptr<rules::Player> player)
    : rules::Api<GameState, error>(std::move(game_state), player)
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
    // TODO
    abort();
}
std::vector<plante> Api::plantes_arrosables(int jardinier)
{
    // TODO
    abort();
}
std::vector<plante> Api::plantes_adultes(int jardinier)
{
    // TODO
    abort();
}
std::vector<plante> Api::plantes_depotables(int jardinier)
{
    // TODO
    abort();
}
std::vector<int> Api::ressources_sur_case(position pos)
{
    // TODO
    abort();
}
bool Api::reproduction_possible(position pos, int rayon_collecte, std::vector<int> consommation)
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
int Api::score(int id_jardinier)
{
    // TODO
    abort();
}
int Api::moi()
{
    // TODO
    abort();
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
    // TODO
    abort();
}
void Api::afficher_erreur(erreur v)
{
    // TODO
    abort();
}
void Api::afficher_action_type(action_type v)
{
    // TODO
    abort();
}
void Api::afficher_caracteristique(caracteristique v)
{
    // TODO
    abort();
}
void Api::afficher_debug_chien(debug_chien v)
{
    // TODO
    abort();
}
void Api::afficher_position(position v)
{
    // TODO
    abort();
}
void Api::afficher_plante(plante v)
{
    // TODO
    abort();
}
void Api::afficher_action_hist(action_hist v)
{
    // TODO
    abort();
}
