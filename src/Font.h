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

    std::string const& GetName() const { return name; }
    Texture2D* GetAtlas() const { return atlas; }
    float GetSize() const { return size; }
    float GetLineHeight() const { return lineHeight; }
    float GetLineHeightNorm() const { return lineHeightNorm; }
    int GetScaleW() const { return scaleW; }
    int GetScaleH() const { return scaleH; }
    Glyph* GetGlyph(int id) const;

private:
    typedef std::map<int /* id */, Glyph*> GlyphMap;

    std::string name;
    Texture2D* atlas;
    float size;
    float lineHeight;
    float lineHeightNorm;
    int scaleW;
    int scaleH;
    GlyphMap glyphs;
};
