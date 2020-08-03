// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

// TODO: test HORS_TOUR

#include <algorithm>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "../constant.hh"
#include "../rules.hh"

static const std::string test_map = R"(
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,3 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,3 0,0,3 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,0 8,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,3 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,3 8,0,3 8,0,0 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,3 8,0,0 8,0,0 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,3 8,0,3 8,0,3 8,0,0 8,0,0 8,0,0 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,3 8,0,3 8,0,0 8,0,0 8,0,0 8,0,0 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,3 8,0,0 8,0,0 8,0,0 8,0,0 8,0,0 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 8,0,0 8,0,0 8,0,0 8,0,0 8,0,0 8,0,0 8,0,0 8,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 999,999,999 999,999,999 999,999,999
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 999,999,999 999,999,999 999,999,999
    0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 0,0,0 999,999,999 999,999,999 999,999,999
    5
    17 18 110 60 0 44 2 0 110 0
    18 17 112 62 2 2 2 0 113 0
    10 10 100 51 0 3 1 0 0 0
    16 10 100 51 0 3 1 0 0 0
    17 10 100 51 0 3 1 0 0 0
    6
    2 2 110 60 42 8 5 0 0 0
    18 19 112 62 2 30 2 0 0 0
    19 18 112 62 2 2 2 0 0 0
    11 11 100 51 0 3 1 0 0 0
    17 11 100 51 0 3 1 0 0 0
    18 11 100 51 0 3 1 0 0 0
)";

template <typename T>
bool same_sets(const std::vector<T>& set1, const std::vector<T>& set2)
{
    std::vector<T> all_items;
    all_items.insert(all_items.end(), set1.begin(), set1.end());
    all_items.insert(all_items.end(), set2.begin(), set2.end());

    for (position pos : all_items)
        if (std::count(set1.begin(), set1.end(), pos) !=
            std::count(set2.begin(), set2.end(), pos))
            return false;

    return true;
}

// Create two players (no spectator)
static rules::Players make_players(int id_p1, int id_p2)
{
    rules::Players players;
    players.add(std::make_shared<rules::Player>(id_p1, rules::PLAYER));
    players.add(std::make_shared<rules::Player>(id_p2, rules::PLAYER));
    return players;
}

static GameState* make_test_gamestate(std::string map,
                                      const rules::Players& players)
{
    std::istringstream map_stream(map);
    return new GameState(map_stream, players);
}

class ApiTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        current_player = 0;
        int player_id_1 = 1337;
        int player_id_2 = 42;

        auto gs_players = make_players(player_id_1, player_id_2);
        std::unique_ptr<GameState> st(
            make_test_gamestate(test_map, gs_players));
        st->start_round();

        players[0].id = player_id_1;
        players[1].id = player_id_2;

        players[0].api = std::make_unique<Api>(
            std::unique_ptr<GameState>(st->copy()), gs_players[0]);
        players[1].api = std::make_unique<Api>(
            std::unique_ptr<GameState>(st->copy()), gs_players[1]);

        p_api = {players[0].api, players[1].api};
    }

    void next_turn()
    {
        for (auto api : p_api)
        {
            api->game_state().end_player_turn(current_player);
            api->game_state().new_player_turn();

            if (current_player == 1)
            {
                api->game_state().end_round();
                api->game_state().start_round();
            }
        }

        current_player = 1 - current_player;
    }

    struct Player
    {
        int id;
        std::shared_ptr<Api> api;
    };

    int current_player;
    std::array<Player, 2> players;
    std::array<std::shared_ptr<Api>, 2> p_api;
};
