#pragma once

#include <string>
#include <map>

class Glyph;
class Texture2D;

class Font
{
public:
    Font(std::string const& name, Texture2D* atlas);
    ~Font();

    bool LoadFromFile(std::string const& file);

private:
    void LoadInfoLine(std::string const& line);
    void LoadCommonLine(std::string const& line);
    void LoadCharLine(std::string const& line);

    typedef std::map<int /* id */, Glyph*> GlyphMap;

    std::string name;
    Texture2D* atlas;
    float size;
    float lineHeight;
    int scaleW;
    int scaleH;
    GlyphMap glyphs;
};
