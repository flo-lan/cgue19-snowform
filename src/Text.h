#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include "Font.h"

// Code snippets taken from https://learnopengl.com/In-Practice/Text-Rendering
class Text
{
public:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
    static class Text& getInstance()
    {
        static class Text instance;
        return instance;
    }
public:
    // We want to make sure these two methods are unacceptable,
    // otherwise we may accidentally get copies of our singleton
    Text(Text const&) = delete;
    void operator=(Text const&) = delete;

    bool Start();
    Font* CreateFontFromFile(std::string const& filePath);
private:
    Text();

    FT_Library ft;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

#define sText Text::getInstance()
