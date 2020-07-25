#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <rules/player.hh>

#include "constant.hh"
#include "map.hh"
#include "utils.hh"
#include "history.hh"

class PlayerInfo
{
public:
    PlayerInfo(std::shared_ptr<rules::Player> player);

    // Getters
    int get_key() const;

    // History
    const std::vector<internal_action>& get_internal_history() const;
    void add_internal_action(internal_action action);
    void reset_internal_history();

    void update_score(const Map& map);

private:
    int score_;
};
