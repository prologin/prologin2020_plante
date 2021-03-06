// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <rules/game-state.hh>
#include <rules/player.hh>

#include "map.hh"
#include "player_info.hh"

const int NB_PLAYERS = 2;

class GameState final : public rules::GameState
{
public:
    GameState(std::istream& map_stream, const rules::Players& players);
    GameState(const GameState& st);
    GameState* copy() const override;

    // Getters
    const Map& get_map() const;
    Map& get_map();
    int get_round() const;
    bool is_finished() const;
    const PlayerInfo& get_player_by_key(int player_key) const;
    PlayerInfo& get_player_by_key(int player_key);

    // Player ids manipulation
    bool player_valid(int player_id) const;
    int get_player_id_by_key(int player_key) const;
    int get_player_key_by_id(int player_id) const;
    int get_opponent_id(int player_id) const;
    const std::array<PlayerInfo, 2>& get_player_info() const;

    // History
    const std::vector<internal_action>&
    get_internal_history(int player_id) const;
    const std::vector<action_hist> get_history(int player_id) const;

    // Init
    bool is_init() const;
    void set_init(bool init);
    void start_round();
    void end_round();
    void new_player_turn(int player_id);
    void end_player_turn(int player_id);

private:
    Map map_;
    std::array<PlayerInfo, NB_PLAYERS> players_;
    int round_;
    bool init_;
};
