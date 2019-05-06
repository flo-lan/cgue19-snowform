#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>

class Shader;

class ShaderProgram
{
public:
    ShaderProgram(std::string const& name);
    ~ShaderProgram();

    bool AttachShader(Shader* shader);
    bool Link();
    void Use();

    GLuint GetUniformLocation(const GLchar* name);

    void SetUniform1i(GLint location, int value);
    void SetUniform1fv(GLint location, float value);
    void SetUniform3fv(GLint location, glm::vec3 value);
    void SetUniform4fv(GLint location, glm::vec4 value);
    void SetUniformMatrix4fv(GLint location, glm::mat4 value);

    std::string const& GetName() const { return Name; }
    GLuint GetHandle() const { return handle; }

private:
    std::string Name;
    GLuint handle;
};
