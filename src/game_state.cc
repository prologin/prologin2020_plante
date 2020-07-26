// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#include "game_state.hh"

std::array<PlayerInfo, NB_PLAYERS> init_players(const rules::Players& players)
{
    std::vector<PlayerInfo> result;
    result.reserve(NB_PLAYERS);

    for (const auto& player : players)
        if (player->type == rules::PLAYER)
            result.emplace_back(player);

    if (result.size() != NB_PLAYERS)
        FATAL("this game requires exactly two players");

    return {result[0], result[1]};
}

GameState::GameState(std::istream& map_stream, const rules::Players& players)
    : rules::GameState()
    , map_(map_stream)
    , players_(init_players(players))
    , init_(false)
    , round_(0)
{
}

GameState::GameState(const GameState& st)
    : rules::GameState()
    , map_(st.map_)
    , players_(st.players_)
    , init_(st.init_)
    , round_(st.round_)
{
}

GameState* GameState::copy() const
{
    return new GameState(*this);
}

const Map& GameState::get_map() const
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

bool GameState::is_finished() const
{
    return round_ >= NB_TOURS;
}

bool GameState::player_valid(int player_key) const
{
    for (int player_id = 0; player_id < 2; player_id++)
        if (get_player_key_by_id(player_id) == player_key)
            return true;

    assertm(false, "unknown player_key");
}

int GameState::get_player_id_by_key(int player_key) const
{
    for (int player_id = 0; player_id < 2; player_id++)
        if (get_player_key_by_id(player_id) == player_key)
            return player_id;

    assertm(false, "unknown player_key");
}

int GameState::get_player_key_by_id(int player_id) const
{
    assert(player_id == 0 || player_id == 1);
    return players_[player_id].get_key();
}

const PlayerInfo& GameState::get_player_by_key(int player_key) const
{
    for (int player_id = 0; player_id < 2; player_id++)
        if (get_player_key_by_id(player_id) == player_key)
            return players_[player_id];

    assertm(false, "unknown player_key");
}

PlayerInfo& GameState::get_player_by_key(int player_key)
{
    for (int player_id = 0; player_id < 2; player_id++)
        if (get_player_key_by_id(player_id) == player_key)
            return players_[player_id];

    assertm(false, "unknown player_key");
}

int GameState::get_opponent_id(int player_id) const
{
    assertm(player_id == 0 || player_id == 1, "wrong player id");
    return 1 - player_id;
}

const std::vector<internal_action>&
GameState::get_internal_history(int player_id) const
{
    assertm(player_id == 0 || player_id == 1, "wrong player id");
    return players_[player_id].get_internal_history();
}

const std::vector<action_hist> GameState::get_history(int player_id) const
{
    assertm(player_id == 0 || player_id == 1, "wrong player id");

    std::vector<action_hist> hist;
    std::vector<internal_action> internal_hist =
        get_internal_history(player_id);

    for (auto action : internal_hist)
        if (action.type == standard_action)
            hist.push_back(action.action);

    return hist;
}

bool GameState::is_init() const
{
    return init_;
}

void GameState::set_init(bool init)
{
    init_ = init;
}

void GameState::start_round()
{
    init_ = true;
}

void GameState::end_round()
{
    ++round_;
}

void GameState::new_player_turn()
{
    map_.new_player_turn();
}

void GameState::end_player_turn(int player)
{
    PlayerInfo& player_ = get_player_by_key(player);
    player_.update_score(map_);
    map_.end_player_turn(player_.get_key());
}
