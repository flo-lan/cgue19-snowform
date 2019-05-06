#include "StringUtils.h"
#include <iostream>
#include <sstream>
#include <regex>

std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void combine_whitespaces(std::string& s)
{
    static std::regex re("\\s{2,}");
    s = std::regex_replace(s, re, " ");
}

void read_key_value_pairs(std::string const& s, std::unordered_map<std::string, std::string>& m, int offset)
{
    std::string::size_type key_pos = 0;
    std::string::size_type key_end = 0;
    std::string::size_type val_pos = 0;
    std::string::size_type val_end = (std::string::size_type)offset;

    do
    {
        if ((key_pos = s.find_first_not_of(" ", val_end)) == std::string::npos)
        {
            break;
        }

        if ((key_end = s.find('=', key_pos)) == std::string::npos)
        {
            break;
        }

        if ((val_pos = s.find_first_not_of("=", key_end)) == std::string::npos)
        {
            break;
        }

        val_end = s.find(' ', val_pos);

        m.emplace(s.substr(key_pos, key_end - key_pos), s.substr(val_pos, val_end - val_pos));
    }
    while (true);
}
