// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <rules/api.hh>
#include <rules/player.hh>

#include "actions.hh"
#include "constant.hh"
#include "game_state.hh"

/**
 * The methods of this class are exported through 'interface.cc'
 * to be called by the clients
 */
class Api final : public rules::Api<GameState, erreur>
{
public:
    Api(std::unique_ptr<GameState> game_state,
        std::shared_ptr<rules::Player> player);
    ~Api() {}

    /// La plante creuse vers une destination donnée
    ApiActionFunc<ActionDepoter> depoter{this};

    /// Arrose une plante
    ApiActionFunc<ActionArroser> arroser{this};

    /// Une plante en gifle une autre
    ApiActionFunc<ActionBaffer> baffer{this};

    /// Affiche le chien spécifié sur la case indiquée
    ApiActionFunc<ActionDebugAfficherChien> debug_afficher_chien{this};

    /// Renvoie la liste des plantes du jardinier
    std::vector<plante> plantes_jardinier(int jardinier);

    /// Renvoie la plante sur la position donnée, s'il n'y en a pas tous les
    /// champs sont initialisés à -1
    plante plante_sur_case(position pos);

    /// Renvoie la liste des plantes du jardinier qui peuvent être arrosées
    std::vector<plante> plantes_arrosables(int jardinier);

    /// Renvoie la liste des plantes du jardinier qui sont adultes
    std::vector<plante> plantes_adultes(int jardinier);

    /// Renvoie la liste des plantes du jardinier qui peuvent être dépotées
    std::vector<plante> plantes_depotables(int jardinier);

    /// Renvoie les ressources disponibles sur une case donnée
    std::vector<int> ressources_sur_case(position pos);

    /// Vérifie si une plante à la position donnée aura suffisamment de
    /// ressources pour se reproduire. S'il n'y a pas déjà une plante, le
    /// calcul supposera qu'une plante est apparu à cette position
    bool reproduction_possible(position pos, int rayon_collecte,
                               std::vector<int> consommation);

    /// Vérifie si une plante à la position donnée peut se reproduire, retourne
    /// faux s'il n'y pas de plante à la position donnée
    bool plante_reproductible(position pos);

    /// Caractéristiques d'une plante résultant du croisement de plusieurs
    /// parents donnés. Les champs sont initialisés à -1 si aucune plante n'est
    /// donnée en paramètre
    plante croisement(std::vector<plante> parents);

    /// Renvoie la liste des actions effectuées par l’adversaire durant son
    /// tour, dans l'ordre chronologique. Les actions de débug n'apparaissent
    /// pas dans cette liste.
    std::vector<action_hist> historique();

    /// Renvoie le score du jardinier ``id_jardinier``. Renvoie -1 si le
    /// jardinier est invalide.
    int score(int id_jardinier);

    /// Renvoie votre numéro de jardinier.
    int moi();

    /// Renvoie le numéro du jardinier adverse.
    int adversaire();

    /// Annule la dernière action. Renvoie faux quand il n'y a pas d'action à
    /// annuler ce tour ci.
    bool annuler();

    /// Retourne le numéro du tour actuel.
    int tour_actuel();

    /// Affiche le contenu d'une valeur de type erreur
    void afficher_erreur(erreur v);

    /// Affiche le contenu d'une valeur de type action_type
    void afficher_action_type(action_type v);

    /// Affiche le contenu d'une valeur de type caracteristique
    void afficher_caracteristique(caracteristique v);

    /// Affiche le contenu d'une valeur de type debug_chien
    void afficher_debug_chien(debug_chien v);

    /// Affiche le contenu d'une valeur de type position
    void afficher_position(position v);

    /// Affiche le contenu d'une valeur de type plante
    void afficher_plante(plante v);

    /// Affiche le contenu d'une valeur de type action_hist
    void afficher_action_hist(action_hist v);
};
