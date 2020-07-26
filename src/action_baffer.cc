// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "utils.hh"

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
    if (st->get_map().hit(position_baffante_, position_baffee_))
    {
        // TODO add internal event plant dies
    }
}

void ActionBaffer::dump_json(const GameState& st, std::ostream& ss) const
{
    // FIXME: optional function that dumps the action
}
