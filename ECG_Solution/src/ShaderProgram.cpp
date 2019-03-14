#include "ShaderProgram.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(std::string const& _name) :
    Name(_name),
    handle(glCreateProgram())
{
    fprintf(stdout, "Created shader program '%s'!\n", Name.c_str());
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(handle);

    fprintf(stdout, "Deleted shader program '%s'!\n", Name.c_str());
}

bool ShaderProgram::AttachShader(Shader* shader)
{
    if (!shader)
    {
        fprintf(stderr, "Could not attach shader to shader program '%s', because shader is null!\n", Name.c_str());
        return false;
    }

    glAttachShader(handle, shader->GetHandle());

    fprintf(stdout, "Attached shader '%s' to shader program '%s'!\n", shader->GetName().c_str(), Name.c_str());

    return true;
}

bool ShaderProgram::Link()
{
    glLinkProgram(handle);

    // Check for shader program linkage errors
    GLint linkStatus = GL_NONE;
    glGetProgramiv(handle, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        GLint logSize = 0;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logSize);

        if (logSize > 0)
        {
            GLchar* logMessage = new GLchar[logSize];
            glGetProgramInfoLog(handle, logSize, nullptr /* length is not required */, logMessage);
            fprintf(stderr, "Could not link shader program '%s', because shader linkage failed: %s\n", Name.c_str(), logMessage);
            delete[] logMessage;
        }
        else
        {
            fprintf(stderr, "Could not link shader program '%s', because shader linkage failed!\n", Name.c_str());
        }

        return false;
    }

    fprintf(stdout, "Linked shader program '%s'!\n", Name.c_str());

    return true;
}

void ShaderProgram::Use()
{
    glUseProgram(handle);
}

GLuint ShaderProgram::GetUniformLocation(const GLchar* name)
{
    return glGetUniformLocation(handle, name);
}

void ShaderProgram::SetUniform1i(GLint location, int value)
{
    glUniform1i(location, value);
}

void ShaderProgram::SetUniform1fv(GLint location, float value)
{
    glUniform1fv(location, 1, &value);
}

void ShaderProgram::SetUniform3fv(GLint location, glm::vec3 value)
{
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::SetUniformMatrix4fv(GLint location, glm::mat4 value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
