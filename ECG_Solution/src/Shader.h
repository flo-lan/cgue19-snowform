#pragma once

#include <GL\glew.h>
#include <string>

class Shader
{
public:
    Shader(std::string const& Name, GLenum type);
    ~Shader();

    bool LoadSourceFromFile(std::string const& filePath);
    bool Compile();

    std::string const& GetName() const { return Name; }
    GLenum GetType() const { return type; }
    GLuint GetHandle() const { return handle; }

private:
    std::string Name;
    GLenum type;
    GLuint handle;
    GLchar* source;
};
