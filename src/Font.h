#pragma once
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <map>

class Font
{
public:
    struct Glyph {
        GLuint TextureID;   // ID handle of the glyph texture
        glm::ivec2 Size;    // Size of glyph
        glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
        GLuint Advance;    // Horizontal offset to advance to next glyph
    };

    Font(const std::map<char, Glyph>& glyphs);

    std::map<GLchar, Glyph> GetGlyphs() const { return glyphs; }
private:
    std::map<GLchar, Glyph> glyphs;
};
