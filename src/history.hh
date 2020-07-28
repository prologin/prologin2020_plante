// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2015 Association Prologin <association@prologin.org>

#pragma once

#include <string>

#include "constant.hh"

enum internal_action_type
{
    flag,
    death,
    birth,
    standard_action,
};

typedef struct flag_info
{
    position pos;
    debug_chien ctype;
} flag_info;

typedef struct death_info
{
    position pos;
} death_info;

typedef struct birth_info
{
    position pos;
} birth_info;

// We do not want unused debug flag info to appear in the public API, so using
// only action_hist is not enough to store the complete internal history.
typedef struct internal_action
{
    internal_action_type type;
    std::string json;

    union {
        flag_info flag;
        death_info death;
        birth_info birth;
        action_hist action;
    };
} internal_action;
