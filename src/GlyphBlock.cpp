#include "GlyphBlock.h"
#include "Glyph.h"
#include "Font.h"

GlyphBlock::GlyphBlock() :
    font(nullptr),
    fontSize(0.f),
    text(""),
    textOverflowMode(TEXT_OVERFLOW_MODE_OVERFLOW),
    wordWrapping(false),
    maxWidth(0.f),
    maxWidthNorm(0.f),
    maxHeight(0.f),
    maxHeightNorm(0.f),
    width(0.f),
    widthNorm(0.f),
    height(0.f),
    heightNorm(0.f)
{
}

GlyphBlock::~GlyphBlock()
{
    ClearGlyphLines();
}

void GlyphBlock::SetFont(Font* font, bool updateGlyphLines)
{
    if (this->font != font)
    {
        this->font = font;

        if (updateGlyphLines)
        {
            UpdateGlyphLines();
        }
    }
}

void GlyphBlock::SetFontSize(float fontSize, bool updateGlyphLines)
{
    if (this->fontSize != fontSize)
    {
        this->fontSize = fontSize;

        if (fontSize > 0.f)
        {
            this->maxWidthNorm = maxWidth / fontSize;
            this->maxHeightNorm = maxHeight / fontSize;
        }

        if (updateGlyphLines)
        {
            UpdateGlyphLines();
        }
    }
}

void GlyphBlock::SetText(std::string const& text, bool updateGlyphLines)
{
    if (this->text != text)
    {
        this->text = text;

        if (updateGlyphLines)
        {
            UpdateGlyphLines();
        }
    }
}

void GlyphBlock::SetTextOverflowMode(TextOverflowMode textOverflowMode, bool updateGlyphLines)
{
    if (this->textOverflowMode != textOverflowMode)
    {
        this->textOverflowMode = textOverflowMode;

        if (updateGlyphLines)
        {
            UpdateGlyphLines();
        }
    }
}

void GlyphBlock::SetWordWrapping(bool enable, bool updateGlyphLines)
{
    if (this->wordWrapping != enable)
    {
        this->wordWrapping = enable;

        if (updateGlyphLines)
        {
            UpdateGlyphLines();
        }
    }
}

void GlyphBlock::SetMaxWidth(float maxWidth, bool updateGlyphLines)
{
    if (this->maxWidth != maxWidth)
    {
        this->maxWidth = maxWidth;

        if (fontSize > 0.f)
        {
            this->maxWidthNorm = maxWidth / fontSize;
        }

        if (updateGlyphLines)
        {
            UpdateGlyphLines();
        }
    }
}

void GlyphBlock::SetMaxHeight(float maxHeight, bool updateGlyphLines)
{
    if (this->maxHeight != maxHeight)
    {
        this->maxHeight = maxHeight;

        if (fontSize > 0.f)
        {
            this->maxHeightNorm = maxHeight / fontSize;
        }

        if (updateGlyphLines)
        {
            UpdateGlyphLines();
        }
    }
}

void GlyphBlock::AddEllipsis(GlyphLine* glyphLine)
{
    if (!glyphLine)
    {
        return;
    }

    if (!font)
    {
        return;
    }

    Glyph* glyph = font->GetGlyph('.');

    if (!glyph)
    {
        fprintf(stderr, "Could not add ellipsis to glyph line, because '.' glyph could not be found in font '%s'!\n", font->GetName().c_str());
        return;
    }

    float ellipsisWidthNorm = 3.f * glyph->GetXAdvanceNorm(); // Width of three dots

    if (textOverflowMode != TEXT_OVERFLOW_MODE_OVERFLOW &&
        textOverflowMode != TEXT_OVERFLOW_MODE_OVERFLOW_H)
    {
        Glyph* lastRemovedGlyph = nullptr;

        // Remove glyphs to make space for ellipsis
        while ((maxWidthNorm - glyphLine->WidthNorm) < ellipsisWidthNorm)
        {
            if (glyphLine->Glyphs.size() == 0)
            {
                break;
            }

            lastRemovedGlyph = glyphLine->Glyphs.back();
            glyphLine->WidthNorm -= lastRemovedGlyph->GetXAdvanceNorm();
            glyphLine->Glyphs.pop_back();
        }
    }

    for (int i = 0; i < 3; i++)
    {
        glyphLine->Glyphs.push_back(glyph);
    }

    glyphLine->WidthNorm += ellipsisWidthNorm;

    if (glyphLine->WidthNorm > widthNorm)
    {
        widthNorm = glyphLine->WidthNorm;
    }
}

void GlyphBlock::UpdateGlyphLines()
{
    if (!font)
    {
        fprintf(stderr, "Could not update glyph lines of glyph block '%s', because font is null!\n", text.c_str());
        return;
    }

    if (fontSize <= 0.f)
    {
        fprintf(stderr, "Could not update glyph lines of glyph block '%s', because font size is <= 0.f!\n", text.c_str());
        return;
    }

    if (textOverflowMode != TEXT_OVERFLOW_MODE_OVERFLOW)
    {
        if (textOverflowMode != TEXT_OVERFLOW_MODE_OVERFLOW_H && maxWidth <= 0.f)
        {
            fprintf(stderr, "Could not update glyph lines of glyph block '%s', because max width is <= 0.f!\n", text.c_str());
            return;
        }

        if (textOverflowMode != TEXT_OVERFLOW_MODE_OVERFLOW_V && maxHeight <= 0.f)
        {
            fprintf(stderr, "Could not update glyph lines of glyph block '%s', because max height is <= 0.f!\n", text.c_str());
            return;
        }
    }

    width = 0;
    height = 0;

    widthNorm = 0.f;
    heightNorm = 0.f;

    int lines = 0;
    float lineHeightNorm = font->GetLineHeightNorm();
    GlyphLine* glyphLine = nullptr;

    for (int i = 0, characters = text.size(); i < characters; i++)
    {
        if (textOverflowMode != TEXT_OVERFLOW_MODE_OVERFLOW &&
            textOverflowMode != TEXT_OVERFLOW_MODE_OVERFLOW_V)
        {
            // No space for next line
            if ((maxHeightNorm - heightNorm) < lineHeightNorm)
            {
                switch (textOverflowMode)
                {
                    case TEXT_OVERFLOW_MODE_ELLIPSIS:
                        AddEllipsis(glyphLine);
                        break;
                    case TEXT_OVERFLOW_MODE_TRUNCATE:
                        // Do nothing
                        break;
                }

                break; // Stop loop
            }
        }

        if (lines < (int)glyphLines.size())
        {
            // Reuse existing glyph line object
            glyphLine = glyphLines[lines];
        }
        else
        {
            // Create new glyph line object
            glyphLines.push_back(glyphLine = new GlyphLine());
        }

        glyphLine->Text = "";
        glyphLine->Glyphs.clear();
        glyphLine->WidthNorm = 0.f;
        glyphLine->HeightNorm = lineHeightNorm;
        glyphLine->WordCount = 0;

        lines++;

        for (char currChar, lastChar = 0; i < characters; i++)
        {
            currChar = text[i];

            if (currChar == '\n')
            {
                break;
            }

            Glyph* glyph = font->GetGlyph(currChar);

            if (!glyph)
            {
                fprintf(stderr, "Could not add glyph to glyph line, because '%c' glyph could not be found in font '%s'!\n", currChar, font->GetName().c_str());
                continue;
            }

            glyphLine->Text += currChar;
            glyphLine->Glyphs.push_back(glyph);
            glyphLine->WidthNorm += glyph->GetXAdvanceNorm();

            // Count words
            if ((lastChar == 0 || IsWordSeparator(lastChar)) && !IsWordSeparator(currChar))
            {
                glyphLine->WordCount++;
            }

            if (textOverflowMode == TEXT_OVERFLOW_MODE_OVERFLOW ||
                textOverflowMode == TEXT_OVERFLOW_MODE_OVERFLOW_H ||
                glyphLine->WidthNorm <= maxWidthNorm)
            {
                lastChar = currChar;
                continue;
            }

            // EOL

            // Each line must at least contain one glyph
            if (glyphLine->Glyphs.size() == 1) {
                break;
            }

            if (wordWrapping)
            {
                if (IsWordSeparator(currChar))
                {
                    // Remove the word separator
                    glyph = glyphLine->Glyphs.back();
                    glyphLine->WidthNorm -= glyph->GetXAdvanceNorm();
                    glyphLine->Glyphs.pop_back();
                    // Move the word separator to the
                    // next line (except whitespace).
                    if (currChar != ' ')
                    {
                        i--;
                    }
                }
                else if (glyphLine->WordCount >= 2)
                {
                    // Remove the last word
                    do
                    {
                        glyph = glyphLine->Glyphs.back();
                        glyphLine->WidthNorm -= glyph->GetXAdvanceNorm();
                        glyphLine->Glyphs.pop_back();
                        i--;
                    } while (!IsWordSeparator((char)glyph->GetId()));

                    // Re-add word separator (except whitespace)
                    if ((char)glyph->GetId() != ' ')
                    {
                        glyphLine->Glyphs.push_back(glyph);
                        glyphLine->WidthNorm += glyph->GetXAdvanceNorm();
                    }

                    // Skip word separator
                    i++;

                    // Decrease word count
                    glyphLine->WordCount--;
                }
                else // Word count == 1
                {
                    // Either the line begins with multiple word separators
                    // and takes all the space, or the word is bigger than
                    // the available line width.

                    // Remove the last character
                    glyph = glyphLine->Glyphs.back();
                    glyphLine->WidthNorm -= glyph->GetXAdvanceNorm();
                    glyphLine->Glyphs.pop_back();
                    // Decrease character counter, because the last
                    // character should be placed in the next line.
                    i--;
                }
            }
            else
            {
                // Remove the last glyph
                glyph = glyphLine->Glyphs.back();
                glyphLine->WidthNorm -= glyph->GetXAdvanceNorm();
                glyphLine->Glyphs.pop_back();
                // Decrease character counter, because the last
                // character should be placed in the next line.
                i--;
            }

            break; // Stop inner loop
        }

        // Remove EOL whitespaces
        while (glyphLine->Glyphs.size() > 0)
        {
            Glyph* glyph = glyphLine->Glyphs.back();

            if ((char)glyph->GetId() != ' ')
            {
                break;
            }

            glyphLine->WidthNorm -= glyph->GetXAdvanceNorm();
            glyphLine->Glyphs.pop_back();
        }

        if (glyphLine->WidthNorm > widthNorm)
        {
            widthNorm = glyphLine->WidthNorm;
        }

        heightNorm += glyphLine->HeightNorm;
    }

    // Delete unused glyph lines
    for (int linesToDelete = glyphLines.size() - lines; linesToDelete > 0; linesToDelete--)
    {
        glyphLine = glyphLines.back();
        glyphLines.pop_back();
        delete glyphLine;
    }

    // Calculate width and height
    width = widthNorm * fontSize;
    height = heightNorm * fontSize;
}

void GlyphBlock::ClearGlyphLines()
{
    for (GlyphLineList::const_iterator itr = glyphLines.begin(); itr != glyphLines.end(); ++itr)
    {
        delete (*itr);
    }

    glyphLines.clear();
}
