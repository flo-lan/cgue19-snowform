#pragma once

#include <GL\glew.h>
#include <string>

class DDSImage;

class Texture2D
{
public:
    Texture2D(std::string const& name);
    ~Texture2D();

    bool LoadFromFile(std::string const& filePath, bool generateMipMaps = true);

    void ActivateAndBind(int unit);

private:
    std::string name;
    DDSImage* image;
    GLuint handle;
};
