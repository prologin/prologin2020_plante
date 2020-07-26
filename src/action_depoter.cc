// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "utils.hh"

int ActionDepoter::check(const GameState& st) const
{
    if (!st.is_init())
        return HORS_TOUR;
    if (!position_in_bounds(position_depart_) ||
        !position_in_bounds(position_arrivee_))
        return HORS_POTAGER;
    if (st.get_map().plant_at(position_arrivee_))
        return CASE_OCCUPEE;
    if (!st.get_map().plant_at(position_depart_))
        return PAS_DE_PLANTE;
    if (st.get_map().plant_at(position_depart_)->jardinier != player_id_)
        return MAUVAIS_JARDINIER;
    if (st.get_map().plant_at(position_depart_)->enracinee)
        return SANS_POT;
    if (!st.get_map().plant_at(position_depart_)->adulte)
        return PAS_ENCORE_ARROSEE;
    // TODO check range
    return OK;
}

void ActionDepoter::apply_on(GameState* st) const
{
    st->get_map().move_plant(position_depart_, position_arrivee_);
    st->get_map().plant_at(position_arrivee_)->enracinee = true;

    PlayerInfo& player_ = st->get_player_by_key(player_id_);
    internal_action action;
    action.type = standard_action;
    action.action.atype = ACTION_DEPOTER;
    action.action.position_baffante = {-1, -1};
    action.action.position_baffee = {-1, -1};
    action.action.position_depart = position_depart_;
    action.action.position_arrivee = position_arrivee_;
    action.action.position_plante = {-1, -1};
    action.action.amelioration = (caracteristique) -1;
    player_.add_internal_action(action);
}

void ActionDepoter::dump_json(const GameState& st, std::ostream& ss) const
{
    // TODO
}
