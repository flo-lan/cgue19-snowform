#pragma once

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
};
