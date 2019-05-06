#include "Glyph.h"

Glyph::Glyph() :
    id(0),
    x(0),
    y(0),
    w(0),
    h(0),
    xOffset(0),
    yOffset(0),
    xAdvance(0),
    wNorm(0.f),
    hNorm(0.f),
    xOffsetNorm(0.f),
    yOffsetNorm(0.f),
    xAdvanceNorm(0.f),
    uv00(0.f, 0.f),
    uv01(0.f, 0.f),
    uv10(0.f, 0.f),
    uv11(0.f, 0.f)
{
}
