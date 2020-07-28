#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <rules/player.hh>

#include "constant.hh"
#include "history.hh"
#include "map.hh"
#include "utils.hh"

class PlayerInfo
{
public:
    PlayerInfo(std::shared_ptr<rules::Player> player);

    // Getters
    int get_key() const;
    int get_score() const;
    const std::string& get_name() const;

    // History
    const std::vector<internal_action>& get_internal_history() const;
    void add_internal_action(internal_action action);
    void reset_internal_history();

    void update_score(const Map& map);
    void sync_score();

private:
    std::shared_ptr<rules::Player> rules_player_;
    std::vector<internal_action> internal_hist_;

    int score_;
};
