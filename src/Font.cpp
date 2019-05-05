#include "Font.h"
#include "Glyph.h"
#include "Texture2D.h"
#include "StringUtils.h"
#include <fstream>
#include <iostream>

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
    std::unordered_map<std::string, std::string> m;
    std::unordered_map<std::string, std::string>::const_iterator itr;

    while (std::getline(fileStream, line))
    {
        if (line.rfind("info", 0) == 0)
        {
            read_key_value_pairs(line, m, 5 /* Offset: "info " */);

            if ((itr = m.find("size")) != m.end())
            {
                size = (float)atof(itr->second.c_str());
            }
        }
        else if (line.rfind("common", 0) == 0)
        {
            read_key_value_pairs(line, m, 7 /* Offset: "common " */);

            if ((itr = m.find("lineHeight")) != m.end())
            {
                lineHeight = (float)atof(itr->second.c_str());
            }

            if ((itr = m.find("scaleW")) != m.end())
            {
                scaleW = atoi(itr->second.c_str());
            }

            if ((itr = m.find("scaleH")) != m.end())
            {
                scaleH = atoi(itr->second.c_str());
            }
        }
        else if (line.rfind("char id", 0) == 0)
        {
            read_key_value_pairs(line, m, 5 /* Offset: "char " */);

            Glyph* glyph = new Glyph();

            if ((itr = m.find("id")) != m.end())
            {
                glyph->id = atoi(itr->second.c_str());
            }

            if ((itr = m.find("x")) != m.end())
            {
                glyph->x = atoi(itr->second.c_str());
            }

            if ((itr = m.find("y")) != m.end())
            {
                glyph->y = atoi(itr->second.c_str());
            }

            if ((itr = m.find("width")) != m.end())
            {
                glyph->w = atoi(itr->second.c_str());
            }

            if ((itr = m.find("height")) != m.end())
            {
                glyph->h = atoi(itr->second.c_str());
            }

            if ((itr = m.find("xOffset")) != m.end())
            {
                glyph->xOffset = atoi(itr->second.c_str());
            }

            if ((itr = m.find("yOffset")) != m.end())
            {
                glyph->yOffset = atoi(itr->second.c_str());
            }

            if ((itr = m.find("xAdvance")) != m.end())
            {
                glyph->xAdvance = atoi(itr->second.c_str());
            }

            glyphs[glyph->id] = glyph;
        }

        m.clear();
    }

    fprintf(stdout, "Loaded font '%s' and %i glyphs from file '%s'!\n", name.c_str(), (int)glyphs.size(), file.c_str());

    fileStream.close();

    return true;
}
