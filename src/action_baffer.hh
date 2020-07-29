// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionBaffer : public rules::Action<GameState> {
public:
  ActionBaffer(position position_baffante, position position_baffee,
               int player_id)
      : position_baffante_(position_baffante),
        position_baffee_(position_baffee), player_id_(player_id) {}
  ActionBaffer() {} // for register_action()

  int check(const GameState &st) const override;
  void apply_on(GameState *st) const override;
  void dump_json(const GameState &st, std::ostream &ss) const;

  void handle_buffer(utils::Buffer &buf) override {
    buf.handle(position_baffante_);
    buf.handle(position_baffee_);
    buf.handle(player_id_);
  }

  uint32_t player_id() const override { return player_id_; };
  uint32_t id() const override { return ID_ACTION_BAFFER; }

private:
  position position_baffante_;
  position position_baffee_;
  int player_id_;
};
