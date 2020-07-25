// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "utils.hh"

int ActionDepoter::check(const GameState& st) const
{
    if (!st.is_init())
        return HORS_TOUR;
    if (position_out_bound(position_depart_)
            || position_out_bound(position_arrivee_))
        return HORS_POTAGER;


    // FIXME
    return 0;
}

void ActionDepoter::apply_on(GameState* st) const
{
    // FIXME
}

void ActionDepoter::dump_json(const GameState& st, std::ostream& ss) const
{
    // FIXME: optional function that dumps the action
}
