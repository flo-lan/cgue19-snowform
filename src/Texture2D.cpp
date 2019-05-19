#include "Texture2D.h"
#include "Utils.h"

Texture2D::Texture2D(std::string const& _name) :
    name(_name),
    image(nullptr),
    handle(0)
{
    fprintf(stdout, "Created texture '%s'!\n", name.c_str());
}

Texture2D::~Texture2D()
{
    fprintf(stdout, "Deleted texture '%s'!\n", name.c_str());

    if (image)
    {
        glDeleteTextures(1, &handle);

        delete image;
    }
}

bool Texture2D::LoadFromFile(std::string const& filePath, bool generateMipMaps)
{
    if (image)
    {
        fprintf(stderr, "Could not load texture '%s' from file '%s', because texture is already loaded!\n", name.c_str(), filePath.c_str());
        return false;
    }

    DDSImage img = loadDDS(filePath.c_str());

    if (!img.image || !img.width || !img.height)
    {
        fprintf(stderr, "Could not load texture '%s' from file '%s', because file could not be loaded!\n", name.c_str(), filePath.c_str());
        return false;
    }

    // Create OpenGL texture
    glGenTextures(1, &handle);

    // All future functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, handle);

    // Upload image to GPU
    glCompressedTexImage2D(GL_TEXTURE_2D, 0 /* Mipmap level */, img.format, img.width, img.height, 0 /* Border */, img.size, img.image);

    // Texture parameters
    if (generateMipMaps)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);               // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // When MINifying the image, use LINEAR filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // When MAGnifying the image, use LINEAR filtering
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Generate Mipmaps
    if (generateMipMaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    return image = new DDSImage(std::move(img));
}

void Texture2D::ActivateAndBind(int unit)
{
    // Activate texture unit
    glActiveTexture(GL_TEXTURE0 + unit);
    // Bind current texture to activated texture unit
    glBindTexture(GL_TEXTURE_2D, handle);
}
