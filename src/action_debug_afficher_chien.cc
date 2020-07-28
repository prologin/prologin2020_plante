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

void ActionDebugAfficherChien::apply_on(GameState* st) const
{
    PlayerInfo& player_ = st->get_player_by_key(player_id_);
    internal_action action;
    action.type = flag;
    action.flag.pos = pos_;
    action.flag.ctype = chien_;
    std::ostringstream stream;
    this->dump_json(*st, stream);
    action.json = stream.str();
    player_.add_internal_action(action);
}

void ActionDebugAfficherChien::dump_json(const GameState& /* st */,
                                         std::ostream& ss) const
{
    ss << "{ \"action_type\": \"afficher_debug_chien\", \"position\": ";
    to_json(ss, pos_);
    ss << ", \"debug_chien\": \"" << chien_ << "\"}";
}
