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
    if (st.get_map_const().plant_at(position_arrivee_))
        return CASE_OCCUPEE;
    if (!st.get_map_const().plant_at(position_depart_))
        return PAS_DE_PLANTE;
    if (st.get_map_const().plant_at(position_depart_)->jardinier != player_id_)
        return MAUVAIS_JARDINIER;
    if (st.get_map_const().plant_at(position_depart_)->enracinee)
        return SANS_POT;
    if (!st.get_map_const().plant_at(position_depart_)->adulte)
        return PAS_ENCORE_ARROSEE;
    // TODO check range
    return OK;
}

void ActionDepoter::apply_on(GameState* st) const
{
    st->get_map().move_plant(position_depart_, position_arrivee_);
    st->get_map().plant_at(position_arrivee_)->enracinee = true;
}

void ActionDepoter::dump_json(const GameState& st, std::ostream& ss) const
{
    // FIXME: optional function that dumps the action
}
