// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "utils.hh"

int ActionArroser::check(const GameState& st) const
{
    if (!st.is_init())
        return HORS_TOUR;
    if (!position_in_bounds(position_plante_))
        return HORS_POTAGER;
    if (!st.get_map().plant_at(position_plante_))
        return PAS_DE_PLANTE;
    if (st.get_map().plant_at(position_plante_)->jardinier != player_id_)
        return MAUVAIS_JARDINIER;
    if (st.get_map().plant_at(position_plante_)->adulte)
        return DEJA_ARROSEE;
    if (st.get_map().plant_at(position_plante_)->age < AGE_DE_POUSSE)
        return PAS_ENCORE_ADULTE;
    if (amelioration_ < 0 || (int)amelioration_ >= 4)
        return CARACTERISTIQUE_INVALIDE;
    return OK;
}

void ActionArroser::apply_on(GameState* st) const
{
    plante plant = *st->get_map().plant_at(position_plante_);
    plant.adulte = true;

    switch (amelioration_)
    {
    case CARACTERISTIQUE_FORCE:
        plant.force += APPORT_CARACTERISTIQUE;
        break;
    case CARACTERISTIQUE_VIE:
        plant.vie_max += APPORT_CARACTERISTIQUE;
        plant.vie += APPORT_CARACTERISTIQUE;
        break;
    case CARACTERISTIQUE_ELEGANCE:
        plant.elegance += APPORT_CARACTERISTIQUE;
        break;
    case CARACTERISTIQUE_RAYON_DEPOTAGE:
        plant.rayon_deplacement += APPORT_CARACTERISTIQUE;
        break;
    }

    st->get_map().update_plant(plant);

    PlayerInfo& player_ = st->get_player_by_key(player_id_);
    internal_action action;
    action.type = standard_action;
    action.action.atype = ACTION_ARROSER;
    action.action.position_baffante = {-1, -1};
    action.action.position_baffee = {-1, -1};
    action.action.position_depart = {-1, -1};
    action.action.position_arrivee = {-1, -1};
    action.action.position_plante = position_plante_;
    action.action.amelioration = amelioration_;
    std::ostringstream stream;
    this->dump_json(*st, stream);
    action.json = stream.str();
    player_.add_internal_action(action);
}

void ActionArroser::dump_json(const GameState& /* st */, std::ostream& ss) const
{
    ss << "{ \"action_type\": \"arroser\", \"position\": ";
    to_json(ss, position_plante_);
    ss << ", \"amelioration\": \"" << amelioration_ << "\"}";
}
