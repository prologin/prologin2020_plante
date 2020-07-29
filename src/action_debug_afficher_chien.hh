// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <rules/action.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

class ActionDebugAfficherChien : public rules::Action<GameState> {
public:
  ActionDebugAfficherChien(position pos, debug_chien chien, int player_id)
      : pos_(pos), chien_(chien), player_id_(player_id) {}
  ActionDebugAfficherChien() {} // for register_action()

  int check(const GameState &st) const override;
  void apply_on(GameState *st) const override;
  void dump_json(const GameState &st, std::ostream &ss) const;

  void handle_buffer(utils::Buffer &buf) override {
    buf.handle(pos_);
    buf.handle(chien_);
    buf.handle(player_id_);
  }

  uint32_t player_id() const override { return player_id_; };
  uint32_t id() const override { return ID_ACTION_DEBUG_AFFICHER_CHIEN; }

private:
  position pos_;
  debug_chien chien_;
  int player_id_;
};
