// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

//! Types and conversions for the C interface
//!
//! Please use the tools defined in `api.rs` to interact with the API for
//! prologin2020_plante.

#![allow(clippy::unit_arg)]
#![allow(clippy::unused_unit)]

use crate::api;

use std::borrow::Borrow;
use std::ffi::{CStr, CString};
use std::{mem::{drop, size_of}, ptr, slice};
use std::os::raw::{c_char, c_double, c_int, c_void};

#[allow(non_camel_case_types)]
pub type c_bool = bool;

/// Stechec2-specific array type.
#[repr(C)]
pub struct Array<T> {
    ptr: *mut T,
    len: usize,
}

impl<T> Drop for Array<T> {
    fn drop(&mut self) {
        unsafe {
            slice::from_raw_parts_mut(self.ptr, self.len)
                .iter_mut()
                .for_each(drop);
            free(self.ptr as _);
        }
    }
}

/// Represents an owned C string that was created by a foreign function using
/// malloc. This means that this string must be deallocated using free.
#[repr(C)]
pub struct RawString {
    ptr: *mut c_char
}

impl Drop for RawString {
    fn drop(&mut self) {
        unsafe {
            free(self.ptr as _);
        }
    }
}

// Enums

#[repr(C)]
#[derive(Clone, Copy)]
pub enum Erreur {
    Ok,
    HorsTour,
    HorsPotager,
    CaseOccupee,
    PasDePlante,
    MauvaisJardinier,
    SansPot,
    DejaArrosee,
    DejaBaffee,
    PasEncoreArrosee,
    PasEncoreAdulte,
    PlanteInvalide,
    TropLoin,
    CaracteristiqueInvalide,
    ChienInvalide,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub enum ActionType {
    ActionDepoter,
    ActionBaffer,
    ActionArroser,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub enum Caracteristique {
    CaracteristiqueForce,
    CaracteristiqueVie,
    CaracteristiqueElegance,
    CaracteristiqueRayonDepotage,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub enum DebugChien {
    AucunChien,
    ChienBleu,
    ChienVert,
    ChienRouge,
}

// Structures

#[repr(C)]
pub struct Position {
    x: c_int,
    y: c_int,
}

#[repr(C)]
pub struct Plante {
    pos: Position,
    jardinier: c_int,
    adulte: c_bool,
    enracinee: c_bool,
    vie: c_int,
    vie_max: c_int,
    force: c_int,
    elegance: c_int,
    rayon_deplacement: c_int,
    rayon_collecte: c_int,
    consommation: Array<c_int>,
    age: c_int,
}

#[repr(C)]
pub struct ActionHist {
    atype: ActionType,
    position_baffante: Position,
    position_baffee: Position,
    position_depart: Position,
    position_arrivee: Position,
    position_plante: Position,
    amelioration: Caracteristique,
}

// Conversion traits

pub trait CToRust<T> {
    /// Convert from a C-compatible type.
    ///
    /// As there can't be a clear ownership through the ffi, you need to make
    /// sure that foreign code assumes that you will drop provided values.
    unsafe fn to_rust(self) -> T;
}

pub trait RustToC<T> {
    /// Convert to a C-compatible type.
    ///
    /// As there can't be a clear ownership through the ffi, you need to make
    /// sure that foreign code assumes that you will drop provided values.
    unsafe fn to_c(&self) -> T;
}

// Conversions for bool

impl CToRust<bool> for c_bool {
    unsafe fn to_rust(self) -> bool {
        self
    }
}

impl RustToC<c_bool> for bool {
    unsafe fn to_c(&self) -> c_bool {
        *self
    }
}

// Conversions for double

impl CToRust<f64> for c_double {
    unsafe fn to_rust(self) -> f64 {
        self
    }
}

impl RustToC<c_double> for f64 {
    unsafe fn to_c(&self) -> c_double {
        *self
    }
}

// Conversions for int

impl CToRust<i32> for c_int {
    unsafe fn to_rust(self) -> i32 {
        self
    }
}

impl RustToC<c_int> for i32 {
    unsafe fn to_c(&self) -> c_int {
        *self
    }
}

// Conversions for void

impl CToRust<()> for () {
    unsafe fn to_rust(self) -> () {
        self
    }
}

impl RustToC<()> for () {
    unsafe fn to_c(&self) -> () {
        *self
    }
}

// Conversions for string

impl CToRust<String> for RawString {
    unsafe fn to_rust(self) -> String {
        CStr::from_ptr(self.ptr)
            .to_owned()
            .into_string()
            .expect("API provided invalid UTF-8")
    }
}

impl<S> RustToC<RawString> for S
where
    S: AsRef<str>,
{
    unsafe fn to_c(&self) -> RawString {
        let c_string = CString::new(self.as_ref().to_string())
            .expect("string provided to the API contains a null character");
        let len = c_string.as_bytes_with_nul().len();

        let ptr = malloc(len * size_of::<c_char>()) as *mut c_char;
        c_string.as_c_str().as_ptr().copy_to(ptr, len);
        RawString { ptr }
    }
}

// Conversions for array

pub unsafe fn array_of_borrow_to_c<T, U, V>(data: &[T]) -> Array<V>
where
    T: Borrow<U>,
    U: RustToC<V>,
{
    let ptr = malloc(data.len() * size_of::<V>()) as *mut V;

    for (i, item) in data.iter().enumerate() {
        ptr::write(ptr.add(i), item.borrow().to_c());
    }

    Array { ptr, len: data.len() }
}

impl<T, U> CToRust<Vec<U>> for Array<T>
where
    T: CToRust<U>,
{
    unsafe fn to_rust(self) -> Vec<U> {
        (0..self.len)
            .map(|i| self.ptr.add(i).read())
            .map(|item| item.to_rust())
            .collect()
    }
}

impl<T, U> RustToC<Array<U>> for [T]
where
    T: RustToC<U>,
{
    unsafe fn to_c(&self) -> Array<U> {
        array_of_borrow_to_c(self)
    }
}

impl<T, U> RustToC<Array<U>> for Vec<T>
where
    T: RustToC<U>,
{
    unsafe fn to_c(&self) -> Array<U> {
        self[..].to_c()
    }
}

// Conversions for erreur

impl CToRust<api::Erreur> for Erreur {
    unsafe fn to_rust(self) -> api::Erreur {
        match self {
            Erreur::Ok => api::Erreur::Ok,
            Erreur::HorsTour => api::Erreur::HorsTour,
            Erreur::HorsPotager => api::Erreur::HorsPotager,
            Erreur::CaseOccupee => api::Erreur::CaseOccupee,
            Erreur::PasDePlante => api::Erreur::PasDePlante,
            Erreur::MauvaisJardinier => api::Erreur::MauvaisJardinier,
            Erreur::SansPot => api::Erreur::SansPot,
            Erreur::DejaArrosee => api::Erreur::DejaArrosee,
            Erreur::DejaBaffee => api::Erreur::DejaBaffee,
            Erreur::PasEncoreArrosee => api::Erreur::PasEncoreArrosee,
            Erreur::PasEncoreAdulte => api::Erreur::PasEncoreAdulte,
            Erreur::PlanteInvalide => api::Erreur::PlanteInvalide,
            Erreur::TropLoin => api::Erreur::TropLoin,
            Erreur::CaracteristiqueInvalide => api::Erreur::CaracteristiqueInvalide,
            Erreur::ChienInvalide => api::Erreur::ChienInvalide,
        }
    }
}

impl RustToC<Erreur> for api::Erreur {
    unsafe fn to_c(&self) -> Erreur {
        match self {
            api::Erreur::Ok => Erreur::Ok,
            api::Erreur::HorsTour => Erreur::HorsTour,
            api::Erreur::HorsPotager => Erreur::HorsPotager,
            api::Erreur::CaseOccupee => Erreur::CaseOccupee,
            api::Erreur::PasDePlante => Erreur::PasDePlante,
            api::Erreur::MauvaisJardinier => Erreur::MauvaisJardinier,
            api::Erreur::SansPot => Erreur::SansPot,
            api::Erreur::DejaArrosee => Erreur::DejaArrosee,
            api::Erreur::DejaBaffee => Erreur::DejaBaffee,
            api::Erreur::PasEncoreArrosee => Erreur::PasEncoreArrosee,
            api::Erreur::PasEncoreAdulte => Erreur::PasEncoreAdulte,
            api::Erreur::PlanteInvalide => Erreur::PlanteInvalide,
            api::Erreur::TropLoin => Erreur::TropLoin,
            api::Erreur::CaracteristiqueInvalide => Erreur::CaracteristiqueInvalide,
            api::Erreur::ChienInvalide => Erreur::ChienInvalide,
        }
    }
}

// Conversions for action_type

impl CToRust<api::ActionType> for ActionType {
    unsafe fn to_rust(self) -> api::ActionType {
        match self {
            ActionType::ActionDepoter => api::ActionType::ActionDepoter,
            ActionType::ActionBaffer => api::ActionType::ActionBaffer,
            ActionType::ActionArroser => api::ActionType::ActionArroser,
        }
    }
}

impl RustToC<ActionType> for api::ActionType {
    unsafe fn to_c(&self) -> ActionType {
        match self {
            api::ActionType::ActionDepoter => ActionType::ActionDepoter,
            api::ActionType::ActionBaffer => ActionType::ActionBaffer,
            api::ActionType::ActionArroser => ActionType::ActionArroser,
        }
    }
}

// Conversions for caracteristique

impl CToRust<api::Caracteristique> for Caracteristique {
    unsafe fn to_rust(self) -> api::Caracteristique {
        match self {
            Caracteristique::CaracteristiqueForce => api::Caracteristique::CaracteristiqueForce,
            Caracteristique::CaracteristiqueVie => api::Caracteristique::CaracteristiqueVie,
            Caracteristique::CaracteristiqueElegance => api::Caracteristique::CaracteristiqueElegance,
            Caracteristique::CaracteristiqueRayonDepotage => api::Caracteristique::CaracteristiqueRayonDepotage,
        }
    }
}

impl RustToC<Caracteristique> for api::Caracteristique {
    unsafe fn to_c(&self) -> Caracteristique {
        match self {
            api::Caracteristique::CaracteristiqueForce => Caracteristique::CaracteristiqueForce,
            api::Caracteristique::CaracteristiqueVie => Caracteristique::CaracteristiqueVie,
            api::Caracteristique::CaracteristiqueElegance => Caracteristique::CaracteristiqueElegance,
            api::Caracteristique::CaracteristiqueRayonDepotage => Caracteristique::CaracteristiqueRayonDepotage,
        }
    }
}

// Conversions for debug_chien

impl CToRust<api::DebugChien> for DebugChien {
    unsafe fn to_rust(self) -> api::DebugChien {
        match self {
            DebugChien::AucunChien => api::DebugChien::AucunChien,
            DebugChien::ChienBleu => api::DebugChien::ChienBleu,
            DebugChien::ChienVert => api::DebugChien::ChienVert,
            DebugChien::ChienRouge => api::DebugChien::ChienRouge,
        }
    }
}

impl RustToC<DebugChien> for api::DebugChien {
    unsafe fn to_c(&self) -> DebugChien {
        match self {
            api::DebugChien::AucunChien => DebugChien::AucunChien,
            api::DebugChien::ChienBleu => DebugChien::ChienBleu,
            api::DebugChien::ChienVert => DebugChien::ChienVert,
            api::DebugChien::ChienRouge => DebugChien::ChienRouge,
        }
    }
}

// Conversions for position

impl CToRust<(i32, i32)> for Position {
    unsafe fn to_rust(self) -> (i32, i32) {
        (self.x.to_rust(), self.y.to_rust())
    }
}

impl RustToC<Position> for (i32, i32) {
    unsafe fn to_c(&self) -> Position {
        let (x, y) = self;

        Position {
            x: x.to_c(),
            y: y.to_c(),
        }
    }
}

// Conversions for plante

impl CToRust<api::Plante> for Plante {
    unsafe fn to_rust(self) -> api::Plante {
        api::Plante {
            pos: self.pos.to_rust(),
            jardinier: self.jardinier.to_rust(),
            adulte: self.adulte.to_rust(),
            enracinee: self.enracinee.to_rust(),
            vie: self.vie.to_rust(),
            vie_max: self.vie_max.to_rust(),
            force: self.force.to_rust(),
            elegance: self.elegance.to_rust(),
            rayon_deplacement: self.rayon_deplacement.to_rust(),
            rayon_collecte: self.rayon_collecte.to_rust(),
            consommation: self.consommation.to_rust(),
            age: self.age.to_rust(),
        }
    }
}

impl RustToC<Plante> for api::Plante {
    unsafe fn to_c(&self) -> Plante {
        Plante {
            pos: self.pos.to_c(),
            jardinier: self.jardinier.to_c(),
            adulte: self.adulte.to_c(),
            enracinee: self.enracinee.to_c(),
            vie: self.vie.to_c(),
            vie_max: self.vie_max.to_c(),
            force: self.force.to_c(),
            elegance: self.elegance.to_c(),
            rayon_deplacement: self.rayon_deplacement.to_c(),
            rayon_collecte: self.rayon_collecte.to_c(),
            consommation: self.consommation.to_c(),
            age: self.age.to_c(),
        }
    }
}

// Conversions for action_hist

impl CToRust<api::ActionHist> for ActionHist {
    unsafe fn to_rust(self) -> api::ActionHist {
        api::ActionHist {
            atype: self.atype.to_rust(),
            position_baffante: self.position_baffante.to_rust(),
            position_baffee: self.position_baffee.to_rust(),
            position_depart: self.position_depart.to_rust(),
            position_arrivee: self.position_arrivee.to_rust(),
            position_plante: self.position_plante.to_rust(),
            amelioration: self.amelioration.to_rust(),
        }
    }
}

impl RustToC<ActionHist> for api::ActionHist {
    unsafe fn to_c(&self) -> ActionHist {
        ActionHist {
            atype: self.atype.to_c(),
            position_baffante: self.position_baffante.to_c(),
            position_baffee: self.position_baffee.to_c(),
            position_depart: self.position_depart.to_c(),
            position_arrivee: self.position_arrivee.to_c(),
            position_plante: self.position_plante.to_c(),
            amelioration: self.amelioration.to_c(),
        }
    }
}


// Import API functions

extern {
    fn free(ptr: *mut c_void);
    fn malloc(size: usize) -> *mut c_void;

    pub fn depoter(position_depart: Position, position_arrivee: Position) -> Erreur;
    pub fn arroser(position_plante: Position, amelioration: Caracteristique) -> Erreur;
    pub fn baffer(position_baffante: Position, position_baffee: Position) -> Erreur;
    pub fn debug_afficher_chien(pos: Position, chien: DebugChien) -> Erreur;
    pub fn plantes_jardinier(jardinier: c_int) -> Array<Plante>;
    pub fn plante_sur_case(pos: Position) -> Plante;
    pub fn plantes_arrosables(jardinier: c_int) -> Array<Plante>;
    pub fn plantes_adultes(jardinier: c_int) -> Array<Plante>;
    pub fn plantes_depotables(jardinier: c_int) -> Array<Plante>;
    pub fn ressources_sur_case(pos: Position) -> Array<c_int>;
    pub fn reproduction_possible(pos: Position, rayon_collecte: c_int, consommation: Array<c_int>) -> c_bool;
    pub fn plante_reproductible(pos: Position) -> c_bool;
    pub fn croisement(parents: Array<Plante>) -> Plante;
    pub fn historique() -> Array<ActionHist>;
    pub fn score(id_jardinier: c_int) -> c_int;
    pub fn moi() -> c_int;
    pub fn adversaire() -> c_int;
    pub fn annuler() -> c_bool;
    pub fn tour_actuel() -> c_int;
    pub fn afficher_erreur(v: Erreur);
    pub fn afficher_action_type(v: ActionType);
    pub fn afficher_caracteristique(v: Caracteristique);
    pub fn afficher_debug_chien(v: DebugChien);
    pub fn afficher_position(v: Position);
    pub fn afficher_plante(v: Plante);
    pub fn afficher_action_hist(v: ActionHist);
}

// Export user functions

#[no_mangle]
unsafe extern "C" fn partie_init() {
    crate::partie_init().to_c()
}

#[no_mangle]
unsafe extern "C" fn jouer_tour() {
    crate::jouer_tour().to_c()
}

#[no_mangle]
unsafe extern "C" fn partie_fin() {
    crate::partie_fin().to_c()
}
