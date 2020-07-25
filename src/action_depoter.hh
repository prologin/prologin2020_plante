// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionDepoter : public rules::Action<GameState>
{
public:
    ActionDepoter(position position_depart, position position_arrivee, int player_id)
        : position_depart_(position_depart)
        , position_arrivee_(position_arrivee)
        , player_id_(player_id)
    {
    }
    ActionDepoter() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;
    void dump_json(const GameState& st, std::ostream& ss) const;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(position_depart_);
        buf.handle(position_arrivee_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_DEPOTER; }

private:
    position position_depart_;
    position position_arrivee_;
    int player_id_;
};
