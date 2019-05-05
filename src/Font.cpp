#include "Font.h"
#include "Glyph.h"
#include "Texture2D.h"
#include "StringUtils.h"
#include <fstream>

Font::Font(std::string const& _name, Texture2D* _atlas) :
    name(_name),
    atlas(_atlas),
    size(0.f),
    lineHeight(0.f),
    scaleW(0),
    scaleH(0)
{
}

Font::~Font()
{
    for (GlyphMap::const_iterator itr = glyphs.begin(); itr != glyphs.end(); ++itr)
    {
        delete itr->second;
    }

    glyphs.clear();
}

bool Font::LoadFromFile(std::string const & file)
{
    std::ifstream fileStream(file);

    if (!fileStream.is_open())
    {
        fprintf(stderr, "Could not load font '%s' from file, because file '%s' could not be opened!\n", name.c_str(), file.c_str());
        return false;
    }

    std::string line = "";

    while (std::getline(fileStream, line))
    {
        if (line.rfind("info", 0) == 0)
        {
            LoadInfoLine(line);
        }
        else if (line.rfind("common", 0) == 0)
        {
            LoadCommonLine(line);
        }
        else if (line.rfind("char id", 0) == 0)
        {
            LoadCharLine(line);
        }
    }

    fprintf(stdout, "Loaded font '%s' and %i glyphs from file '%s'!\n", name.c_str(), (int)glyphs.size(), file.c_str());

    fileStream.close();

    return true;
}

void Font::LoadInfoLine(std::string const& line)
{
    std::vector<std::string> pairs = split(combine_whitespaces(line), ' ');

    for (std::vector<std::string>::const_iterator itr = pairs.begin(); itr != pairs.end(); ++itr)
    {
        std::vector<std::string> pair = split(*itr, '=');

        if (pair.size() != 2)
        {
            continue;
        }

        std::string key = pair[0];
        std::string value = pair[1];

        if (key == "size")
        {
            size = atof(value.c_str());
        }
    }
}

void Font::LoadCommonLine(std::string const& line)
{
    std::vector<std::string> pairs = split(combine_whitespaces(line), ' ');

    for (std::vector<std::string>::const_iterator itr = pairs.begin(); itr != pairs.end(); ++itr)
    {
        std::vector<std::string> pair = split(*itr, '=');

        if (pair.size() != 2)
        {
            continue;
        }

        std::string key = pair[0];
        std::string value = pair[1];

        if (key == "lineHeight")
        {
            lineHeight = atof(value.c_str());
        }
        else if (key == "scaleW")
        {
            scaleW = atoi(value.c_str());
        }
        else if (key == "scaleH")
        {
            scaleH = atoi(value.c_str());
        }
    }
}

void Font::LoadCharLine(std::string const& line)
{
    std::vector<std::string> pairs = split(combine_whitespaces(line), ' ');

    Glyph* glyph = new Glyph();

    for (std::vector<std::string>::const_iterator itr = pairs.begin(); itr != pairs.end(); ++itr)
    {
        std::vector<std::string> pair = split(*itr, '=');

        if (pair.size() != 2)
        {
            continue;
        }

        std::string key = pair[0];
        std::string value = pair[1];

        if (key == "id")
        {
            glyph->id = atoi(value.c_str());
        }
        else if (key == "x")
        {
            glyph->x = atoi(value.c_str());
        }
        else if (key == "y")
        {
            glyph->y = atoi(value.c_str());
        }
        else if (key == "width")
        {
            glyph->w = atoi(value.c_str());
        }
        else if (key == "height")
        {
            glyph->h = atoi(value.c_str());
        }
        else if (key == "xOffset")
        {
            glyph->xOffset = atoi(value.c_str());
        }
        else if (key == "yOffset")
        {
            glyph->yOffset = atoi(value.c_str());
        }
        else if (key == "xAdvance")
        {
            glyph->xAdvance = atoi(value.c_str());
        }
    }

    glyphs[glyph->id] = glyph;
}
