// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once


/// Nombre de jardiniers
#define NB_JARDINIERS 2

/// Largeur et hauteur de la grille
#define TAILLE_GRILLE 20

/// Nombre de tours à jouer avant la fin de la partie
#define NB_TOURS 100

/// Durée de vie maximale d'une plante
#define AGE_MAX 10

/// Âge auquel la plante atteint la maturité et peut donc être arrosée
#define AGE_DE_POUSSE 3

/// Portée maximale d'une baffe
#define PORTEE_BAFFE 6

/// Types de ressources existantes
#define NB_TYPES_RESSOURCES 3

/// Apport pour une caractéristique lors de l'arrosage
#define APPORT_CARACTERISTIQUE 10

/// Nombre de points de caractéristiques nécessaires pour pouvoir dépoter une
/// cases plus loins.
#define COUT_PAR_CASE_DEPOTAGE 15


/// Erreurs possibles
typedef enum erreur
{
    OK, ///< L'action s'est effectuée avec succès
    HORS_TOUR, ///< Il est interdit de faire des actions hors de jouer_tour
    HORS_POTAGER, ///< La case désignée n'est pas dans le potager
    CASE_OCCUPEE, ///< Il y a déjà une plante sur la case ciblée
    PAS_DE_PLANTE, ///< Il n'y a pas de plante sur la case ciblée
    MAUVAIS_JARDINIER, ///< La plante n'appartient pas au bon jardinier
    SANS_POT, ///< La plante est déjà dépotée
    DEJA_ARROSEE, ///< La plante a déjà été arrosée
    DEJA_BAFFEE, ///< La plante a déjà baffé ce tour ci
    PAS_ENCORE_ARROSEE, ///< La plante n'as pas encore été arrosée
    PAS_ENCORE_ADULTE, ///< La plante ne peut pas encore être arrosée
    PLANTE_INVALIDE, ///< Les caractéristiques de la plante sont invalides
} erreur;

/// Types d'actions
typedef enum action_type
{
    ACTION_DEPOTER, ///< Action ``depoter``
    ACTION_BAFFER, ///< Action ``baffer``
    ACTION_ARROSER, ///< Action ``arroser``
} action_type;

/// Caractéristiques améliorables d'une plante
typedef enum caracteristique
{
    CARACTERISTIQUE_FORCE, ///< Force
    CARACTERISTIQUE_VIE, ///< Vie 
    CARACTERISTIQUE_ELEGANCE, ///< Élégance
    CARACTERISTIQUE_RAYON_DEPOTAGE, ///< Portée de dépotage
} caracteristique;

/// Types de chien de débug
typedef enum debug_chien
{
    AUCUN_CHIEN, ///< Aucun chien, enlève le chien présent
    CHIEN_BLEU, ///< Chien bleu
    CHIEN_VERT, ///< Chien vert
    CHIEN_ROUGE, ///< Chien rouge
} debug_chien;


/// Position dans le jardin, donnée par deux coordonnées.
typedef struct position
{
    int x; ///< Coordonnée : x
    int y; ///< Coordonnée : y
} position;

/// Une plante
typedef struct plante
{
    position pos; ///< Position de la plante
    int jardinier; ///< Jardinier ayant planté la plante
    bool adulte; ///< La plante est adulte
    bool enracinee; ///< La plante a déjà déjà été dépotée
    int vie; ///< Point(s) de vie restant(s) de la plante
    int vie_max; ///< Point(s) de vie maximumde la plante
    int force; ///< Force de la baffe de la plante
    int elegance; ///< Élégance de la plante
    int rayon_deplacement; ///< Distance maximale parcourable par la plante en creusant
    int rayon_collecte; ///< Rayon de collecte des ressources pour la plante
    std::vector<int> consommation; ///< Quantité de ressources consommées par la plante
    int age; ///< Âge de la plante
} plante;

/// Représentation d'une action dans l'historique
typedef struct action_hist
{
    action_type atype; ///< Type de l'action
    position position_baffante; ///< Position de la plante baffante (si type d'action ``action_baffer``)
    position position_baffee; ///< Position de la plante baffée (si type d'action ``action_baffer``)
    position position_depart; ///< Position de la plante à déplacer (si type d'action ``action_depoter``)
    position position_arrivee; ///< Position où déplacer la plante (si type d'action ``action_depoter``)
    position position_plante; ///< Position de la plante  (si type d'action ``action_arroser``)
    caracteristique amelioration; ///< Caractéristique à améliorer (si type d'action ``action_arroser``)
} action_hist;
