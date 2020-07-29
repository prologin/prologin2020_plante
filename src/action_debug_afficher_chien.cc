// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"

int ActionDebugAfficherChien::check(const GameState &st) const {
  // FIXME
  return 0;
}

void ActionDebugAfficherChien::apply_on(GameState *st) const {
  // FIXME
}

void ActionDebugAfficherChien::dump_json(const GameState &st,
                                         std::ostream &ss) const {
  // FIXME: optional function that dumps the action
}
