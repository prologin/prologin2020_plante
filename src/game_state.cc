// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"

GameState::GameState(std::istream& map_stream, const rules::Players& players)
    : rules::GameState()
    , map_(map_stream)
    , init_(false)
    , round_(0)
{
}

GameState::GameState(const GameState& st)
    : rules::GameState()
    , map_(st.map_)
    , init_(st.init_)
    , round_(st.round_)
{
}

GameState* GameState::copy() const
{
    return new GameState(*this);
}

const Map& GameState::get_map_const() const
{
    return map_;
}

Map& GameState::get_map()
{
    return map_;
}

int GameState::get_round() const
{
    return round_;
}

bool GameState::is_init() const
{
    return init_;
}
