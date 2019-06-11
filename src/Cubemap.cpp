#include "Cubemap.h"
#include "Utils.h"

Cubemap::Cubemap(std::string const& _name) :
    name(_name),
    images{nullptr},
    handle(0)
{
    fprintf(stdout, "Created cubemap '%s'!\n", name.c_str());
}

Cubemap::~Cubemap()
{
    fprintf(stdout, "Deleted cubemap '%s'!\n", name.c_str());

    if (handle)
    {
        glDeleteTextures(1, &handle);
    }

    for (int i = 0; i < NUM_CUBEMAP_TEXTURES; i++)
    {
        delete images[i];
    }
}

bool Cubemap::LoadFromFile(std::string const& leftFilePath,
    std::string const& rightFilePath,
    std::string const& topFilePath,
    std::string const& bottomFilePath,
    std::string const& frontFilePath,
    std::string const& backFilePath)
{
    if (handle)
    {
        return false;
    }

    std::string filePaths[NUM_CUBEMAP_TEXTURES] =
    {
        leftFilePath,
        rightFilePath,
        topFilePath,
        bottomFilePath,
        frontFilePath,
        backFilePath
    };

    static GLuint textureTypes[NUM_CUBEMAP_TEXTURES] =
    {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    for (int i = 0; i < NUM_CUBEMAP_TEXTURES; i++)
    {
        DDSImage img = loadDDS(filePaths[i].c_str());

        if (!img.image || !img.width || !img.height)
        {
            fprintf(stderr, "Could not load cubemap '%s', because file '%s' could not be loaded!\n", name.c_str(), filePaths[i].c_str());
            return false;
        }

        glCompressedTexImage2D(textureTypes[i], 0 /* Mipmap level */, img.format, img.width, img.height, 0 /* Border */, img.size, img.image);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        images[i] = new DDSImage(std::move(img));
    }

    return true;
}

void Cubemap::ActivateAndBind(int unit)
{
    // Activate texture unit
    glActiveTexture(GL_TEXTURE0 + unit);
    // Bind current cubemap to activated texture unit
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
}
