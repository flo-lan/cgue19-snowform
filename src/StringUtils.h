#pragma once

#include <string>
#include <vector>

// Credits: https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> split(const std::string& s, char delimiter);

// Credits: http://www.cplusplus.com/forum/beginner/185953/#msg907135
std::string combine_whitespaces(std::string const& s);
