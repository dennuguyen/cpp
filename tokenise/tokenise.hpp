#pragma once

#include <regex>
#include <vector>

auto tokenise(std::string const& str, std::string const& delim) -> std::vector<std::string> {
    auto const re_delim = std::regex(delim);
    auto begin = std::sregex_token_iterator(str.begin(), str.end(), re_delim, -1);
    auto end = std::sregex_token_iterator();
    return std::vector<std::string>(begin, end);
}