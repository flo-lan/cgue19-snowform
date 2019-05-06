#pragma once

#include <string>
#include <vector>
#include <unordered_map>

// Credits: https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> split(const std::string& s, char delimiter);

// Credits: http://www.cplusplus.com/forum/beginner/185953/#msg907135
void combine_whitespaces(std::string& s);

// Credits: https://stackoverflow.com/a/38814150
void read_key_value_pairs(std::string const& s, std::unordered_map<std::string, std::string>& m, int offset);