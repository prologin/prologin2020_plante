// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

#include "utils.hh"

std::vector<std::string> split(std::string text, const std::string& delim)
{
    std::vector<std::string> parts;
    size_t pos = 0;
    std::string token;

    while ((pos = text.find(delim)) != std::string::npos)
    {
        parts.push_back(text.substr(0, pos));
        text.erase(0, pos + delim.length());
    }

    parts.push_back(text);
    return parts;
}
