#pragma once

#include <glm/glm.hpp>

class Glyph
{
    friend class Font;

public:
    int GetId() const { return id; }
    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetW() const { return w; }
    int GetH() const { return h; }
    int GetXOffset() const { return xOffset; }
    int GetYOffset() const { return yOffset; }
    int GetXAdvance() const { return xAdvance; }
    float GetWNorm() const { return wNorm; }
    float GetHNorm() const { return hNorm; }
    float GetXOffsetNorm() const { return xOffsetNorm; }
    float GetYOffsetNorm() const { return yOffsetNorm; }
    float GetXAdvanceNorm() const { return xAdvanceNorm; }
    glm::vec2 GetUV00() const { return uv00; }
    glm::vec2 GetUV01() const { return uv01; }
    glm::vec2 GetUV10() const { return uv10; }
    glm::vec2 GetUV11() const { return uv11; }

private:
    Glyph();

    int id;
    int x;
    int y;
    int w;
    int h;
    int xOffset;
    int yOffset;
    int xAdvance;
    float wNorm;
    float hNorm;
    float xOffsetNorm;
    float yOffsetNorm;
    float xAdvanceNorm;
    glm::vec2 uv00;
    glm::vec2 uv01;
    glm::vec2 uv10;
    glm::vec2 uv11;
};
