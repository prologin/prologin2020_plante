// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#pragma once

#include <string>
#include <vector>

#include "constant.hh"

std::vector<std::string> split(std::string text, const std::string& delim);
bool position_out_bound(const position& position);
