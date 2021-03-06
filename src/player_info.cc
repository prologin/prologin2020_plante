#include "player_info.hh"

PlayerInfo::PlayerInfo(std::shared_ptr<rules::Player> player)
    : rules_player_(std::move(player))
    , score_(0)
{
    rules_player_->score = 0;
}

int PlayerInfo::get_key() const
{
    return this->rules_player_->id;
}

int PlayerInfo::get_score() const
{
    return score_;
}

const std::string& PlayerInfo::get_name() const
{
    return rules_player_->name;
}

const std::vector<internal_action>& PlayerInfo::get_internal_history() const
{
    return internal_hist_;
}

void PlayerInfo::reset_internal_history()
{
    internal_hist_.clear();
}

void PlayerInfo::add_internal_action(internal_action action)
{
    internal_hist_.push_back(action);
}

void PlayerInfo::update_score(const Map& map)
{
    const auto player_plants = map.all_plants_with([=](const plante& plant) {
        return plant.adulte && plant.jardinier == (int)this->rules_player_->id;
    });

    for (const plante& plant : player_plants)
        score_ += plant.elegance;

    sync_score();
}

void PlayerInfo::sync_score()
{
    rules_player_->score = score_;
}
