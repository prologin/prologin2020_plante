// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include <iostream>
#include <sstream>
#include <vector>

#include "api.hh"

// from api.cc
extern Api* api;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& arr)
{
    os << "[";
    typename std::vector<T>::const_iterator it;
    for (it = arr.begin(); it != arr.end(); ++it)
    {
        if (it != arr.begin())
            os << ", ";
        os << *it;
    }
    os << "]";
    return os;
}


extern "C" erreur api_depoter(position position_depart, position position_arrivee)
{
    return api->depoter(position_depart, position_arrivee);
}

extern "C" erreur api_arroser(position position_plante, caracteristique amelioration)
{
    return api->arroser(position_plante, amelioration);
}

extern "C" erreur api_baffer(position position_baffante, position position_baffee)
{
    return api->baffer(position_baffante, position_baffee);
}

extern "C" erreur api_debug_afficher_chien(position pos, debug_chien chien)
{
    return api->debug_afficher_chien(pos, chien);
}

extern "C" std::vector<plante> api_plantes_jardinier(int jardinier)
{
    return api->plantes_jardinier(jardinier);
}

extern "C" plante api_plante_sur_case(position pos)
{
    return api->plante_sur_case(pos);
}

extern "C" std::vector<plante> api_plantes_arrosables(int joueur)
{
    return api->plantes_arrosables(joueur);
}

extern "C" std::vector<plante> api_plantes_adultes(int joueur)
{
    return api->plantes_adultes(joueur);
}

extern "C" std::vector<plante> api_plantes_depotables(int joueur)
{
    return api->plantes_depotables(joueur);
}

extern "C" std::vector<int> api_ressources_sur_case(position pos)
{
    return api->ressources_sur_case(pos);
}

extern "C" bool api_reproduction_possible(position pos, int rayon_collecte, std::vector<int> consommation)
{
    return api->reproduction_possible(pos, rayon_collecte, consommation);
}

extern "C" bool api_plante_reproductible(position pos)
{
    return api->plante_reproductible(pos);
}

extern "C" plante api_croisement(std::vector<plante> parents)
{
    return api->croisement(parents);
}

extern "C" std::vector<action_hist> api_historique()
{
    return api->historique();
}

extern "C" int api_score(int id_joueur)
{
    return api->score(id_joueur);
}

extern "C" int api_moi()
{
    return api->moi();
}

extern "C" int api_adversaire()
{
    return api->adversaire();
}

extern "C" bool api_annuler()
{
    return api->cancel();
}

extern "C" int api_tour_actuel()
{
    return api->tour_actuel();
}

std::ostream& operator<<(std::ostream& os, erreur v)
{
    switch (v)
    {
    case OK:
        os << "OK";
        break;
    case HORS_TOUR:
        os << "HORS_TOUR";
        break;
    case HORS_POTAGER:
        os << "HORS_POTAGER";
        break;
    case CASE_OCCUPEE:
        os << "CASE_OCCUPEE";
        break;
    case PAS_DE_PLANTE:
        os << "PAS_DE_PLANTE";
        break;
    case MAUVAIS_JARDINIER:
        os << "MAUVAIS_JARDINIER";
        break;
    case SANS_POT:
        os << "SANS_POT";
        break;
    case DEJA_ARROSEE:
        os << "DEJA_ARROSEE";
        break;
    case PAS_ENCORE_ARROSEE:
        os << "PAS_ENCORE_ARROSEE";
        break;
    case PAS_ENCORE_ADULTE:
        os << "PAS_ENCORE_ADULTE";
        break;
    case PLANTE_INVALIDE:
        os << "PLANTE_INVALIDE";
        break;
    }
    return os;
}

extern "C" void api_afficher_erreur(erreur v)
{
    std::cerr << v << std::endl;
}
std::ostream& operator<<(std::ostream& os, action_type v)
{
    switch (v)
    {
    case ACTION_DEPOTER:
        os << "ACTION_DEPOTER";
        break;
    case ACTION_BAFFER:
        os << "ACTION_BAFFER";
        break;
    case ACTION_ARROSER:
        os << "ACTION_ARROSER";
        break;
    }
    return os;
}

extern "C" void api_afficher_action_type(action_type v)
{
    std::cerr << v << std::endl;
}
std::ostream& operator<<(std::ostream& os, caracteristique v)
{
    switch (v)
    {
    case CARACTERISTIQUE_FORCE:
        os << "CARACTERISTIQUE_FORCE";
        break;
    case CARACTERISTIQUE_VIE:
        os << "CARACTERISTIQUE_VIE";
        break;
    case CARACTERISTIQUE_ELEGANCE:
        os << "CARACTERISTIQUE_ELEGANCE";
        break;
    case CARACTERISTIQUE_RAYON_DEPLACEMENT:
        os << "CARACTERISTIQUE_RAYON_DEPLACEMENT";
        break;
    }
    return os;
}

extern "C" void api_afficher_caracteristique(caracteristique v)
{
    std::cerr << v << std::endl;
}
std::ostream& operator<<(std::ostream& os, debug_chien v)
{
    switch (v)
    {
    case AUCUN_DRAPEAU:
        os << "AUCUN_DRAPEAU";
        break;
    case CHIEN_BLEU:
        os << "CHIEN_BLEU";
        break;
    case CHIEN_VERT:
        os << "CHIEN_VERT";
        break;
    case CHIEN_ROUGE:
        os << "CHIEN_ROUGE";
        break;
    }
    return os;
}

extern "C" void api_afficher_debug_chien(debug_chien v)
{
    std::cerr << v << std::endl;
}

std::ostream& operator<<(std::ostream& os, position v)
{
    os << "{ ";
    os << "x"
       << "=" << v.x;
    os << ", ";
    os << "y"
       << "=" << v.y;
    os << " }";
    return os;
}

extern "C" void api_afficher_position(position v)
{
    std::cerr << v << std::endl;
}
std::ostream& operator<<(std::ostream& os, plante v)
{
    os << "{ ";
    os << "pos"
       << "=" << v.pos;
    os << ", ";
    os << "jardinier"
       << "=" << v.jardinier;
    os << ", ";
    os << "adulte"
       << "=" << v.adulte;
    os << ", ";
    os << "enracinee"
       << "=" << v.enracinee;
    os << ", ";
    os << "vie"
       << "=" << v.vie;
    os << ", ";
    os << "vie_max"
       << "=" << v.vie_max;
    os << ", ";
    os << "force"
       << "=" << v.force;
    os << ", ";
    os << "elegance"
       << "=" << v.elegance;
    os << ", ";
    os << "rayon_deplacement"
       << "=" << v.rayon_deplacement;
    os << ", ";
    os << "rayon_collecte"
       << "=" << v.rayon_collecte;
    os << ", ";
    os << "consommation"
       << "=" << v.consommation;
    os << ", ";
    os << "age"
       << "=" << v.age;
    os << " }";
    return os;
}

extern "C" void api_afficher_plante(plante v)
{
    std::cerr << v << std::endl;
}
std::ostream& operator<<(std::ostream& os, action_hist v)
{
    os << "{ ";
    os << "atype"
       << "=" << v.atype;
    os << ", ";
    os << "position_baffante"
       << "=" << v.position_baffante;
    os << ", ";
    os << "position_baffee"
       << "=" << v.position_baffee;
    os << ", ";
    os << "position_depart"
       << "=" << v.position_depart;
    os << ", ";
    os << "position_arrivee"
       << "=" << v.position_arrivee;
    os << ", ";
    os << "position_plante"
       << "=" << v.position_plante;
    os << ", ";
    os << "amelioration"
       << "=" << v.amelioration;
    os << " }";
    return os;
}

extern "C" void api_afficher_action_hist(action_hist v)
{
    std::cerr << v << std::endl;
}
