// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "rules.hh"

#include <fstream>
#include <memory>
#include <utility>

#include "actions.hh"

Rules::Rules(const rules::Options opt)
    : TurnBasedRules(opt)
    , sandbox_(opt.time)
{
    if (!opt.champion_lib.empty())
    {
        champion_dll_ = std::make_unique<utils::DLL>(opt.champion_lib);

        // FIXME: register user functions
    }

    std::ifstream ifs(opt.map_file);

    if (!ifs.is_open())
        FATAL("Cannot open file: %s", opt.map_file.c_str());

    auto game_state = std::make_unique<GameState>(ifs, opt.players);
    api_ = std::make_unique<Api>(std::move(game_state), opt.player);
    register_actions();
}

void Rules::register_actions()
{
    api_->actions()->register_action(ID_ACTION_DEPOTER,
                                     [] { return std::make_unique<ActionDepoter>(); });
    api_->actions()->register_action(ID_ACTION_ARROSER,
                                     [] { return std::make_unique<ActionArroser>(); });
    api_->actions()->register_action(ID_ACTION_BAFFER,
                                     [] { return std::make_unique<ActionBaffer>(); });
    api_->actions()->register_action(ID_ACTION_DEBUG_AFFICHER_CHIEN,
                                     [] { return std::make_unique<ActionDebugAfficherChien>(); });
}

rules::Actions* Rules::get_actions()
{
    return api_->actions();
}

void Rules::apply_action(const rules::IAction& action)
{
    // When receiving an action, the API should have already checked that it
    // is valid. We recheck that for the current gamestate here to avoid weird
    // client/server desynchronizations and make sure the gamestate is always
    // consistent across the clients and the server.

    int err = api_->game_state_check(action);
    if (err)
        FATAL("Synchronization error: received action %d from player %d, but "
              "check() on current gamestate returned %d.",
              action.id(), action.player_id(), err);
    api_->game_state_apply(action);
}

bool Rules::is_finished()
{
    // FIXME
    return true;
}
