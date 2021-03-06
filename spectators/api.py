# This file was generated by stechec2-generator. DO NOT EDIT.

from enum import IntEnum
from collections import namedtuple


# Nombre de jardiniers
NB_JARDINIERS = 2

# Largeur et hauteur de la grille
TAILLE_GRILLE = 20

# Nombre de tours à jouer avant la fin de la partie
NB_TOURS = 100

# Durée de vie maximale d'une plante
AGE_MAX = 10

# Âge auquel la plante atteint la maturité et peut donc être arrosée
AGE_DE_POUSSE = 3

# Portée maximale d'une baffe
PORTEE_BAFFE = 6

# Types de ressources existantes
NB_TYPES_RESSOURCES = 3

# Apport pour une caractéristique lors de l'arrosage
APPORT_CARACTERISTIQUE = 10

# Nombre de points de caractéristiques nécessaires pour pouvoir dépoter une
# cases plus loins.
COUT_PAR_CASE_DEPOTAGE = 15


# Erreurs possibles
class erreur(IntEnum):
    OK = 0  # <- L'action s'est effectuée avec succès
    HORS_TOUR = 1  # <- Il est interdit de faire des actions hors de jouer_tour
    HORS_POTAGER = 2  # <- La case désignée n'est pas dans le potager
    CASE_OCCUPEE = 3  # <- Il y a déjà une plante sur la case ciblée
    PAS_DE_PLANTE = 4  # <- Il n'y a pas de plante sur la case ciblée
    MAUVAIS_JARDINIER = 5  # <- La plante n'appartient pas au bon jardinier
    SANS_POT = 6  # <- La plante est déjà dépotée
    DEJA_ARROSEE = 7  # <- La plante a déjà été arrosée
    DEJA_BAFFEE = 8  # <- La plante a déjà baffé ce tour ci
    PAS_ENCORE_ARROSEE = 9  # <- La plante n'as pas encore été arrosée
    PAS_ENCORE_ADULTE = 10  # <- La plante ne peut pas encore être arrosée
    PLANTE_INVALIDE = 11  # <- Les caractéristiques de la plante sont invalides
    TROP_LOIN = 12  # <- La plante n'a pas un assez grand rayon de dépotage
    CARACTERISTIQUE_INVALIDE = 13  # <- Valeur de `Caracteristique` inconnue
    CHIEN_INVALIDE = 14  # <- Valeur de `Chien` inconnue


# Types d'actions
class action_type(IntEnum):
    ACTION_DEPOTER = 0  # <- Action ``depoter``
    ACTION_BAFFER = 1  # <- Action ``baffer``
    ACTION_ARROSER = 2  # <- Action ``arroser``


# Caractéristiques améliorables d'une plante
class caracteristique(IntEnum):
    CARACTERISTIQUE_FORCE = 0  # <- Force
    CARACTERISTIQUE_VIE = 1  # <- Vie 
    CARACTERISTIQUE_ELEGANCE = 2  # <- Élégance
    CARACTERISTIQUE_RAYON_DEPOTAGE = 3  # <- Portée de dépotage


# Types de chien de débug
class debug_chien(IntEnum):
    AUCUN_CHIEN = 0  # <- Aucun chien, enlève le chien présent
    CHIEN_BLEU = 1  # <- Chien bleu
    CHIEN_VERT = 2  # <- Chien vert
    CHIEN_ROUGE = 3  # <- Chien rouge


# Une plante
plante = namedtuple('plante', [
    'pos',  # Position de la plante
    'jardinier',  # Jardinier ayant planté la plante
    'adulte',  # La plante est adulte
    'enracinee',  # La plante a déjà déjà été dépotée
    'vie',  # Point(s) de vie restant(s) de la plante
    'vie_max',  # Point(s) de vie maximumde la plante
    'force',  # Force de la baffe de la plante
    'elegance',  # Élégance de la plante
    'rayon_deplacement',  # Distance maximale parcourable par la plante en creusant
    'rayon_collecte',  # Rayon de collecte des ressources pour la plante
    'consommation',  # Quantité de ressources consommées par la plante
    'age',  # Âge de la plante
])


# Représentation d'une action dans l'historique
action_hist = namedtuple('action_hist', [
    'atype',  # Type de l'action
    'position_baffante',  # Position de la plante baffante (si type d'action ``action_baffer``)
    'position_baffee',  # Position de la plante baffée (si type d'action ``action_baffer``)
    'position_depart',  # Position de la plante à déplacer (si type d'action ``action_depoter``)
    'position_arrivee',  # Position où déplacer la plante (si type d'action ``action_depoter``)
    'position_plante',  # Position de la plante  (si type d'action ``action_arroser``)
    'amelioration',  # Caractéristique à améliorer (si type d'action ``action_arroser``)
])


