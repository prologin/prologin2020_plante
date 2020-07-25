// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <rules/game-state.hh>
#include <rules/player.hh>

#include "map.hh"

class GameState final : public rules::GameState
{
public:
    GameState(std::istream& map_stream, const rules::Players& players);
    GameState(const GameState& st);
    GameState* copy() const override;

    // Getters
    const Map& get_map() const;

    // Init
    bool is_init() const;


private:
    Map map_;
    int round_;
    bool init_;
};
