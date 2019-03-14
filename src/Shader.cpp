#include "Shader.h"

#include <fstream>

Shader::Shader(std::string const& _name, GLenum _type) :
    Name(_name),
    type(_type),
    handle(glCreateShader(_type)),
    source(nullptr)
{
    fprintf(stdout, "Created shader '%s'!\n", Name.c_str());
}

Shader::~Shader()
{
    delete[] source;
    glDeleteShader(handle);

    fprintf(stdout, "Deleted shader '%s'!\n", Name.c_str());
}

bool Shader::LoadSourceFromFile(std::string const& filePath)
{
    #define READ_BLOCK_BYTES 512

    // Open file in ASCII mode
    std::ifstream ifs(filePath, std::ios::in);
    if (!ifs)
    {
        fprintf(stderr, "Could not load source for shader '%s' from file '%s', because file could not be opened!\n", Name.c_str(), filePath.c_str());
        return false;
    }

    // Read file to the end and ignore result
    ifs.ignore(std::numeric_limits<std::streamsize>::max());

    // Get file size in bytes
    int32_t length = (int32_t)ifs.gcount();

    // Reset, since ignore will have set eof.
    ifs.clear();

    // Set stream position back to the beginning of the file
    ifs.seekg(std::ios::beg);

    // Check if file is empty
    if (length == 0)
    {
        fprintf(stderr, "Could not load source for shader '%s' from file '%s', because file is empty!\n", Name.c_str(), filePath.c_str());
        return false;
    }

    if (source != nullptr)
    {
        delete[] source;
    }

    source = new GLchar[length + 1];

    for (int32_t i = length, j = 0; i > 0 && ifs.good() && !ifs.eof();)
    {
        if (i > READ_BLOCK_BYTES)
        {
            ifs.read(&source[j], READ_BLOCK_BYTES);

            j += READ_BLOCK_BYTES;
            i -= READ_BLOCK_BYTES;
        }
        else
        {
            ifs.read(&source[j], i);

            j += i;
            i = 0;
        }
    }

    // Make sure the last character is 0-terminator
    source[length] = 0;

    fprintf(stdout, "Loaded source for shader '%s' from file '%s'!\n", Name.c_str(), filePath.c_str());

    return true;
}

bool Shader::Compile()
{
    if (source == nullptr)
    {
        fprintf(stderr, "Could not compile shader '%s', because source is null!\n", Name.c_str());
        return false;
    }

    // Attach shader source to shader object
    glShaderSource(handle, 1 /* Only one shader source */, &source, nullptr /* 0-terminated strings */);

    // Compile shader
    glCompileShader(handle);

    // Check for shader compilation errors
    GLint compileStatus = GL_NONE;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus)
    {
        GLint logSize = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logSize);

        if (logSize > 0)
        {
            GLchar* logMessage = new GLchar[logSize];
            glGetShaderInfoLog(handle, logSize, nullptr /* length is not required */, logMessage);
            fprintf(stderr, "Could not compile shader '%s', because shader compilation failed: %s\n", Name.c_str(), logMessage);
            delete[] logMessage;
        }
        else
        {
            fprintf(stderr, "Could not compile shader '%s', because shader compilation failed!\n", Name.c_str());
        }

        return false;
    }

    fprintf(stdout, "Compiled shader '%s'!\n", Name.c_str());

    return true;
}
