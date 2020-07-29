// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"

int ActionBaffer::check(const GameState &st) const {
  // FIXME
  return 0;
}

void ActionBaffer::apply_on(GameState *st) const {
  // FIXME
}

void ActionBaffer::dump_json(const GameState &st, std::ostream &ss) const {
  // FIXME: optional function that dumps the action
}
