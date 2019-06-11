#pragma once

#include <GL\glew.h>
#include <string>

class DDSImage;

enum CubemapTextures
{
    CUBEMAP_TEXTURE_LEFT   = 0,
    CUBEMAP_TEXTURE_RIGHT  = 1,
    CUBEMAP_TEXTURE_TOP    = 2,
    CUBEMAP_TEXTURE_BOTTOM = 3,
    CUBEMAP_TEXTURE_FRONT  = 4,
    CUBEMAP_TEXTURE_BACK   = 5,
    NUM_CUBEMAP_TEXTURES   = 6,
};

class Cubemap
{
public:
    Cubemap(std::string const& name);
    ~Cubemap();

    bool LoadFromFile(std::string const& leftFilePath,
        std::string const& rightFilePath,
        std::string const& topFilePath,
        std::string const& bottomFilePath,
        std::string const& frontFilePath,
        std::string const& backFilePath);

    void ActivateAndBind(int unit);

private:
    std::string name;
    DDSImage* images[NUM_CUBEMAP_TEXTURES];
    GLuint handle;
};
