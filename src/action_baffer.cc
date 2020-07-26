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
}

void ActionBaffer::dump_json(const GameState& st, std::ostream& ss) const
{
    // TODO: optional function that dumps the action
}
