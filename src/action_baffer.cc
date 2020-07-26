// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "utils.hh"
#include "history.hh"

int ActionBaffer::check(const GameState& st) const
{
    if (!st.is_init())
        return HORS_TOUR;
    if (!position_in_bounds(position_baffante_) ||
        !position_in_bounds(position_baffee_))
        return HORS_POTAGER;
    if (!st.get_map().plant_at(position_baffante_))
        return PAS_DE_PLANTE;
    if (st.get_map().plant_at(position_baffante_)->jardinier != player_id_)
        return MAUVAIS_JARDINIER;
    if (!st.get_map().plant_at(position_baffante_)->adulte)
        return PAS_ENCORE_ARROSEE;
    if (st.get_map().already_hit(position_baffante_))
        ; // TODO add deja baffée ce tour

    return OK;
}

void ActionBaffer::apply_on(GameState* st) const
{
    PlayerInfo& player_ = st->get_player_by_key(player_id_);
    if (st->get_map().hit(position_baffante_, position_baffee_))
    {
        internal_action death_action;
        death_action.type = death;
        death_action.death.pos = position_baffee_;

        player_.add_internal_action(death_action);
    }

    internal_action action;
    action.type = standard_action;
    action.action.atype = ACTION_BAFFER;
    action.action.position_baffante = position_baffante_;
    action.action.position_baffee = position_baffee_;
    action.action.position_depart = {-1, -1};
    action.action.position_arrivee = {-1, -1};
    action.action.position_plante = {-1, -1};
    action.action.amelioration = (caracteristique) -1;
    player_.add_internal_action(action);
}

void ActionBaffer::dump_json(const GameState& st, std::ostream& ss) const
{
    ss << "{ \"action_type\": \"baffer\", \"position_baffante\": ";
    to_json(ss, position_baffante_);
    ss << ", \"position_baffée\": ";
    to_json(ss, position_baffee_);
    ss << "}";
}
