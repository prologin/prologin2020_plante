// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "utils.hh"

int ActionDebugAfficherChien::check(const GameState& st) const
{
    if (!position_in_bounds(pos_))
        return HORS_POTAGER;
    // TODO check chien_
    return OK;
}

void ActionDebugAfficherChien::apply_on(GameState* st) const {}

void ActionDebugAfficherChien::dump_json(const GameState& st,
                                         std::ostream& ss) const
{
    // TODO: optional function that dumps the action
}
