// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>
//
// This file contains all the API functions for the Rust language, and all the
// constants, enumerations and structures.
//
// It has been generated. You can modify the end of the file if you want, but
// do not touch the part where constants, enums, structs, and api functions are
// defined.

//! Rust API for prologin2020_plante

use crate::ffi;
use crate::ffi::{array_of_borrow_to_c, CToRust, RustToC};

#[allow(unused_imports)]
use std::{cell::UnsafeCell, borrow::Borrow};

/// Nombre de jardiniers
pub const NB_JARDINIERS: i32 = 2;

/// Largeur et hauteur de la grille
pub const TAILLE_GRILLE: i32 = 20;

/// Nombre de tours à jouer avant la fin de la partie
pub const NB_TOURS: i32 = 100;

/// Durée de vie maximale d'une plante
pub const AGE_MAX: i32 = 10;

/// Âge auquel la plante atteint la maturité et peut donc être arrosée
pub const AGE_DE_POUSSE: i32 = 3;

/// Portée maximale d'une baffe
pub const PORTEE_BAFFE: i32 = 6;

/// Types de ressources existantes
pub const NB_TYPES_RESSOURCES: i32 = 3;

/// Apport pour une caractéristique lors de l'arrosage
pub const APPORT_CARACTERISTIQUE: i32 = 10;

/// Nombre de points de caractéristiques nécessaires pour pouvoir dépoter une
/// cases plus loins.
pub const COUT_PAR_CASE_COLLECTE: i32 = 15;

/// Nombre de points de caractéristiques nécessaires pour pouvoir dépoter une
/// cases plus loins.
pub const COUT_PAR_CASE_DEPOTAGE: i32 = 15;

#[derive(Clone, Copy, Debug, Eq, Hash, PartialEq, PartialOrd, Ord)]
pub enum Erreur {
    /// L'action s'est effectuée avec succès
    Ok,
    /// Il est interdit de faire des actions hors de jouer_tour
    HorsTour,
    /// La case désignée n'est pas dans le potager
    HorsPotager,
    /// Il y a déjà une plante sur la case ciblée
    CaseOccupee,
    /// Il n'y a pas de plante sur la case ciblée
    PasDePlante,
    /// La plante n'appartient pas au bon jardinier
    MauvaisJardinier,
    /// La plante est déjà dépotée
    SansPot,
    /// La plante a déjà été arrosée
    DejaArrosee,
    /// La plante a déjà baffé ce tour ci
    DejaBaffee,
    /// La plante n'as pas encore été arrosée
    PasEncoreArrosee,
    /// La plante ne peut pas encore être arrosée
    PasEncoreAdulte,
    /// Les caractéristiques de la plante sont invalides
    PlanteInvalide,
    /// La plante n'a pas un assez grand rayon de dépotage
    TropLoin,
    /// Valeur de `Caracteristique` inconnue
    CaracteristiqueInvalide,
    /// Valeur de `Chien` inconnue
    ChienInvalide,
}

#[derive(Clone, Copy, Debug, Eq, Hash, PartialEq, PartialOrd, Ord)]
pub enum ActionType {
    /// Action ``depoter``
    ActionDepoter,
    /// Action ``baffer``
    ActionBaffer,
    /// Action ``arroser``
    ActionArroser,
}

#[derive(Clone, Copy, Debug, Eq, Hash, PartialEq, PartialOrd, Ord)]
pub enum Caracteristique {
    /// Force
    CaracteristiqueForce,
    /// Vie 
    CaracteristiqueVie,
    /// Élégance
    CaracteristiqueElegance,
    /// Portée de dépotage
    CaracteristiqueRayonDepotage,
}

#[derive(Clone, Copy, Debug, Eq, Hash, PartialEq, PartialOrd, Ord)]
pub enum DebugChien {
    /// Aucun chien, enlève le chien présent
    AucunChien,
    /// Chien bleu
    ChienBleu,
    /// Chien vert
    ChienVert,
    /// Chien rouge
    ChienRouge,
}

#[derive(Clone, Debug, Eq, Hash, Ord, PartialEq, PartialOrd)]
pub struct Plante {
    /// Position de la plante
    pub pos: (i32, i32),
    /// Jardinier ayant planté la plante
    pub jardinier: i32,
    /// La plante est adulte
    pub adulte: bool,
    /// La plante a déjà déjà été dépotée
    pub enracinee: bool,
    /// Point(s) de vie restant(s) de la plante
    pub vie: i32,
    /// Point(s) de vie maximumde la plante
    pub vie_max: i32,
    /// Force de la baffe de la plante
    pub force: i32,
    /// Élégance de la plante
    pub elegance: i32,
    /// Distance maximale parcourable par la plante en creusant
    pub rayon_deplacement: i32,
    /// Rayon de collecte des ressources pour la plante
    pub rayon_collecte: i32,
    /// Quantité de ressources consommées par la plante
    pub consommation: Vec<i32>,
    /// Âge de la plante
    pub age: i32,
}

#[derive(Clone, Copy, Debug, Eq, Hash, Ord, PartialEq, PartialOrd)]
pub struct ActionHist {
    /// Type de l'action
    pub atype: ActionType,
    /// Position de la plante baffante (si type d'action ``action_baffer``)
    pub position_baffante: (i32, i32),
    /// Position de la plante baffée (si type d'action ``action_baffer``)
    pub position_baffee: (i32, i32),
    /// Position de la plante à déplacer (si type d'action ``action_depoter``)
    pub position_depart: (i32, i32),
    /// Position où déplacer la plante (si type d'action ``action_depoter``)
    pub position_arrivee: (i32, i32),
    /// Position de la plante  (si type d'action ``action_arroser``)
    pub position_plante: (i32, i32),
    /// Caractéristique à améliorer (si type d'action ``action_arroser``)
    pub amelioration: Caracteristique,
}


/// La plante creuse vers une destination donnée
///
/// ### Parameters
///  - `position_depart`: Position de la plante à déplacer
///  - `position_arrivee`: Position où déplacer la plante
pub fn depoter(position_depart: (i32, i32), position_arrivee: (i32, i32)) -> Erreur {
    unsafe {
        let position_depart = position_depart.to_c();
        let position_arrivee = position_arrivee.to_c();
        ffi::depoter(position_depart, position_arrivee).to_rust()
    }
}

/// Arrose une plante
///
/// ### Parameters
///  - `position_plante`: Position de la plante
///  - `amelioration`: Caractéristique à améliorer
pub fn arroser(position_plante: (i32, i32), amelioration: Caracteristique) -> Erreur {
    unsafe {
        let position_plante = position_plante.to_c();
        let amelioration = amelioration.to_c();
        ffi::arroser(position_plante, amelioration).to_rust()
    }
}

/// Une plante en gifle une autre
///
/// ### Parameters
///  - `position_baffante`: Position de la plante baffante
///  - `position_baffee`: Position de la plante baffée
pub fn baffer(position_baffante: (i32, i32), position_baffee: (i32, i32)) -> Erreur {
    unsafe {
        let position_baffante = position_baffante.to_c();
        let position_baffee = position_baffee.to_c();
        ffi::baffer(position_baffante, position_baffee).to_rust()
    }
}

/// Affiche le chien spécifié sur la case indiquée
///
/// ### Parameters
///  - `pos`: Case ciblée
///  - `chien`: Chien à afficher sur la case
pub fn debug_afficher_chien(pos: (i32, i32), chien: DebugChien) -> Erreur {
    unsafe {
        let pos = pos.to_c();
        let chien = chien.to_c();
        ffi::debug_afficher_chien(pos, chien).to_rust()
    }
}

/// Renvoie la liste des plantes du jardinier
///
/// ### Parameters
///  - `jardinier`: ID du jardinier concerné
pub fn plantes_jardinier(jardinier: i32) -> Vec<Plante> {
    unsafe {
        let jardinier = jardinier.to_c();
        ffi::plantes_jardinier(jardinier).to_rust()
    }
}

/// Renvoie la plante sur la position donnée, s'il n'y en a pas tous les champs
/// sont initialisés à -1
///
/// ### Parameters
///  - `pos`: Case ciblée
pub fn plante_sur_case(pos: (i32, i32)) -> Plante {
    unsafe {
        let pos = pos.to_c();
        ffi::plante_sur_case(pos).to_rust()
    }
}

/// Renvoie la liste des plantes du jardinier qui peuvent être arrosées
///
/// ### Parameters
///  - `jardinier`: ID du jardinier concerné
pub fn plantes_arrosables(jardinier: i32) -> Vec<Plante> {
    unsafe {
        let jardinier = jardinier.to_c();
        ffi::plantes_arrosables(jardinier).to_rust()
    }
}

/// Renvoie la liste des plantes du jardinier qui sont adultes
///
/// ### Parameters
///  - `jardinier`: ID du jardinier concerné
pub fn plantes_adultes(jardinier: i32) -> Vec<Plante> {
    unsafe {
        let jardinier = jardinier.to_c();
        ffi::plantes_adultes(jardinier).to_rust()
    }
}

/// Renvoie la liste des plantes du jardinier qui peuvent être dépotées
///
/// ### Parameters
///  - `jardinier`: ID du jardinier concerné
pub fn plantes_depotables(jardinier: i32) -> Vec<Plante> {
    unsafe {
        let jardinier = jardinier.to_c();
        ffi::plantes_depotables(jardinier).to_rust()
    }
}

/// Renvoie les ressources disponibles sur une case donnée
///
/// ### Parameters
///  - `pos`: Case ciblée
pub fn ressources_sur_case(pos: (i32, i32)) -> Vec<i32> {
    unsafe {
        let pos = pos.to_c();
        ffi::ressources_sur_case(pos).to_rust()
    }
}

/// Vérifie si une plante à la position donnée aura suffisamment de ressources
/// pour se reproduire. S'il y a déjà une plante à cette position, le calcul
/// suposera qu'elle a été remplacée
///
/// ### Parameters
///  - `pos`: Case ciblée
///  - `rayon_collecte`: Rayon de collecte des ressources pour la plante
///  - `consommation`: Quantité de ressources consommées par la plante
pub fn reproduction_possible(pos: (i32, i32), rayon_collecte: i32, consommation: &[impl Borrow<i32>]) -> bool {
    unsafe {
        let pos = pos.to_c();
        let rayon_collecte = rayon_collecte.to_c();
        let consommation = UnsafeCell::new(array_of_borrow_to_c(consommation));
        ffi::reproduction_possible(pos, rayon_collecte, consommation.get().read()).to_rust()
    }
}

/// Vérifie si une plante à la position donnée peut se reproduire, retourne
/// faux s'il n'y pas de plante à la position donnée
///
/// ### Parameters
///  - `pos`: Case ciblée
pub fn plante_reproductible(pos: (i32, i32)) -> bool {
    unsafe {
        let pos = pos.to_c();
        ffi::plante_reproductible(pos).to_rust()
    }
}

/// Caractéristiques d'une plante résultant du croisement de plusieurs parents
/// donnés. Les champs sont initialisés à -1 si aucune plante n'est donnée en
/// paramètre
///
/// ### Parameters
///  - `parents`: Les plantes qui sont croisées
pub fn croisement(parents: &[impl Borrow<Plante>]) -> Plante {
    unsafe {
        let parents = UnsafeCell::new(array_of_borrow_to_c(parents));
        ffi::croisement(parents.get().read()).to_rust()
    }
}

/// Renvoie la liste des actions effectuées par l’adversaire durant son tour,
/// dans l'ordre chronologique. Les actions de débug n'apparaissent pas dans
/// cette liste.
pub fn historique() -> Vec<ActionHist> {
    unsafe {
        ffi::historique().to_rust()
    }
}

/// Renvoie le score du jardinier ``id_jardinier``. Renvoie -1 si le jardinier
/// est invalide.
///
/// ### Parameters
///  - `id_jardinier`: Numéro du jardinier
pub fn score(id_jardinier: i32) -> i32 {
    unsafe {
        let id_jardinier = id_jardinier.to_c();
        ffi::score(id_jardinier).to_rust()
    }
}

/// Renvoie votre numéro de jardinier.
pub fn moi() -> i32 {
    unsafe {
        ffi::moi().to_rust()
    }
}

/// Renvoie le numéro du jardinier adverse.
pub fn adversaire() -> i32 {
    unsafe {
        ffi::adversaire().to_rust()
    }
}

/// Annule la dernière action. Renvoie faux quand il n'y a pas d'action à
/// annuler ce tour ci.
pub fn annuler() -> bool {
    unsafe {
        ffi::annuler().to_rust()
    }
}

/// Retourne le numéro du tour actuel.
pub fn tour_actuel() -> i32 {
    unsafe {
        ffi::tour_actuel().to_rust()
    }
}

/// Affiche le contenu d'une valeur de type erreur
///
/// ### Parameters
///  - `v`: The value to display
pub fn afficher_erreur(v: Erreur) {
    unsafe {
        let v = v.to_c();
        ffi::afficher_erreur(v).to_rust()
    }
}

/// Affiche le contenu d'une valeur de type action_type
///
/// ### Parameters
///  - `v`: The value to display
pub fn afficher_action_type(v: ActionType) {
    unsafe {
        let v = v.to_c();
        ffi::afficher_action_type(v).to_rust()
    }
}

/// Affiche le contenu d'une valeur de type caracteristique
///
/// ### Parameters
///  - `v`: The value to display
pub fn afficher_caracteristique(v: Caracteristique) {
    unsafe {
        let v = v.to_c();
        ffi::afficher_caracteristique(v).to_rust()
    }
}

/// Affiche le contenu d'une valeur de type debug_chien
///
/// ### Parameters
///  - `v`: The value to display
pub fn afficher_debug_chien(v: DebugChien) {
    unsafe {
        let v = v.to_c();
        ffi::afficher_debug_chien(v).to_rust()
    }
}

/// Affiche le contenu d'une valeur de type position
///
/// ### Parameters
///  - `v`: The value to display
pub fn afficher_position(v: (i32, i32)) {
    unsafe {
        let v = v.to_c();
        ffi::afficher_position(v).to_rust()
    }
}

/// Affiche le contenu d'une valeur de type plante
///
/// ### Parameters
///  - `v`: The value to display
pub fn afficher_plante(v: &Plante) {
    unsafe {
        let v = UnsafeCell::new(v.to_c());
        ffi::afficher_plante(v.get().read()).to_rust()
    }
}

/// Affiche le contenu d'une valeur de type action_hist
///
/// ### Parameters
///  - `v`: The value to display
pub fn afficher_action_hist(v: &ActionHist) {
    unsafe {
        let v = UnsafeCell::new(v.to_c());
        ffi::afficher_action_hist(v.get().read()).to_rust()
    }
}
