// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "actions.hh"
#include "utils.hh"

int ActionArroser::check(const GameState& st) const
{
    if (!st.is_init())
        return HORS_TOUR;
    if (position_out_bound(position_plante_))
        return HORS_POTAGER;
    if (!st.get_map().plant_at(position_plante_))
        return PAS_DE_PLANTE;
    if (st.get_map().plant_at(position_plante_)->jardinier != player_id_)
        return MAUVAIS_JARDINIER;
    if (st.get_map().plant_at(position_plante_)->adulte)
        return DEJA_ARROSEE;
    if (st.get_map().plant_at(position_plante_)->age < AGE_DE_POUSSE)
        return PAS_ENCORE_ADULTE;
    if (amelioration_ < 0 || amelioration_ <= NB_TYPES_RESSOURCES)
        return PLANTE_INVALIDE;
    return OK;
}

void ActionArroser::apply_on(GameState* st) const
{
    // FIXME
}

void ActionArroser::dump_json(const GameState& st, std::ostream& ss) const
{
    // FIXME: optional function that dumps the action
}
