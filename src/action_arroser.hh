// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionArroser : public rules::Action<GameState>
{
public:
    ActionArroser(position position_plante, caracteristique amelioration, int player_id)
        : position_plante_(position_plante)
        , amelioration_(amelioration)
        , player_id_(player_id)
    {
    }
    ActionArroser() {} // for register_action()

    int check(const GameState& st) const override;
    void apply_on(GameState* st) const override;
    void dump_json(const GameState& st, std::ostream& ss) const;

    void handle_buffer(utils::Buffer& buf) override
    {
        buf.handle(position_plante_);
        buf.handle(amelioration_);
        buf.handle(player_id_);
    }

    uint32_t player_id() const override { return player_id_; };
    uint32_t id() const override { return ID_ACTION_ARROSER; }

private:
    position position_plante_;
    caracteristique amelioration_;
    int player_id_;
};
