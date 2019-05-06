#pragma once

#include <string>
#include <vector>

class Font;
class Glyph;

enum TextOverflowMode
{
    TEXT_OVERFLOW_MODE_OVERFLOW,
    TEXT_OVERFLOW_MODE_ELLIPSIS,
    TEXT_OVERFLOW_MODE_TRUNCATE,
    TEXT_OVERFLOW_MODE_OVERFLOW_H,
    TEXT_OVERFLOW_MODE_OVERFLOW_V
};

typedef std::vector<Glyph*> GlyphList;

struct GlyphLine
{
    std::string Text;
    GlyphList Glyphs;
    float WidthNorm;
    float HeightNorm;
    int WordCount;

    GlyphLine() :
        Text(""),
        WidthNorm(0.f),
        HeightNorm(0.f),
        WordCount(0)
    {}
};

typedef std::vector<GlyphLine*> GlyphLineList;

class GlyphBlock
{
public:
    GlyphBlock();
    ~GlyphBlock();

    void SetFont(Font* font, bool updateGlyphLines = false);
    void SetFontSize(float fontSize, bool updateGlyphLines = false);
    void SetText(std::string const& text, bool updateGlyphLines = false);
    void SetTextOverflowMode(TextOverflowMode textOverflowMode, bool updateGlyphLines = false);
    void SetWordWrapping(bool enable, bool updateGlyphLines = false);
    void SetMaxWidth(float maxWidth, bool updateGlyphLines = false);
    void SetMaxHeight(float maxHeight, bool updateGlyphLines = false);

    void AddEllipsis(GlyphLine* glyphLine);
    void UpdateGlyphLines();
    void ClearGlyphLines();

    Font* GetFont() const { return font; }
    float GetFontSize() const { return fontSize; }
    std::string const& GetText() const { return text; }
    TextOverflowMode GetTextOverflowMode() const { return textOverflowMode; }
    bool IsWordWrappingEnabled() const { return wordWrapping; }
    float GetMaxWidth() const { return maxWidth; }
    float GetMaxWidthNorm() const { return maxWidthNorm; }
    float GetMaxHeight() const { return maxHeight; }
    float GetMaxHeightNorm() const { return maxHeightNorm; }
    float GetWidth() const { return width; }
    float GetWidthNorm() const { return widthNorm; }
    float GetHeight() const { return height; }
    float GetHeightNorm() const { return heightNorm; }
    GlyphLineList const& GetGlyphLines() const { return glyphLines; }

    static bool IsWordSeparator(char c) { return c == ' ' || c == '-'; }

private:
    Font* font;
    float fontSize;
    std::string text;
    TextOverflowMode textOverflowMode;
    bool wordWrapping;
    float maxWidth;
    float maxWidthNorm;
    float maxHeight;
    float maxHeightNorm;
    float width;
    float widthNorm;
    float height;
    float heightNorm;
    GlyphLineList glyphLines;
};
