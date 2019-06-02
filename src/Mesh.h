#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 UV;
    glm::vec3 Color;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv, glm::vec3 color) :
        Position(position),
        Normal(normal),
        UV(uv),
        Color(color)
    {
    }

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) :
        Vertex(position, normal, uv, glm::vec3(1.f, 1.f, 1.f))
    {
    }

    Vertex(glm::vec3 position, glm::vec3 normal) :
        Vertex(position, normal, glm::vec2())
    {
    }

    Vertex(glm::vec3 position) :
        Vertex(position, glm::vec3(), glm::vec2())
    {
    }

    Vertex(float positionX, float positionY, float positionZ)
        : Vertex(glm::vec3(positionX, positionY, positionZ))
    {
    }

    Vertex(float positionX, float positionY, float positionZ, float normalX, float normalY, float normalZ)
        : Vertex(glm::vec3(positionX, positionY, positionZ), glm::vec3(normalX, normalY, normalZ), glm::vec2())
    {
    }

    Vertex(float positionX, float positionY, float positionZ, float normalX, float normalY, float normalZ, glm::vec2 uv)
        : Vertex(glm::vec3(positionX, positionY, positionZ), glm::vec3(normalX, normalY, normalZ), uv)
    {
    }

    Vertex(float positionX, float positionY, float positionZ, float normalX, float normalY, float normalZ, float u, float v)
        : Vertex(glm::vec3(positionX, positionY, positionZ), glm::vec3(normalX, normalY, normalZ), glm::vec2(u, v))
    {
    }

    Vertex(float positionX, float positionY, float positionZ, float normalX, float normalY, float normalZ, float u, float v, float r, float g, float b)
        : Vertex(glm::vec3(positionX, positionY, positionZ), glm::vec3(normalX, normalY, normalZ), glm::vec2(u, v), glm::vec3(r, g, b))
    {
    }
};

class Mesh
{
public:
    Mesh(std::string const& name);
    ~Mesh();

    void Upload();
    bool IsUploaded() const { return VAO != 0; }

    void Bind();
    void Unbind();

    std::string const& GetName() const { return Name; }

    size_t GetVertexCount() const { return Vertices.size(); }
    size_t GetIndexCount() const { return Indices.size(); }

    static Mesh* CreateQuad(std::string const& name, float width, float height);
    static Mesh* CreateCube(std::string const& name, float width, float height, float depth);
    static Mesh* CreateCylinder(std::string const& name, uint32_t segments, float radius, float height);
    static Mesh* CreateSphere(std::string const& name, uint32_t segmentsLongitude, uint32_t segmentsLatitude, float radius);
    static Mesh* CreateTorus(std::string const& name, uint32_t tubeSegments, uint32_t crossSectionSegments, float r1, float r2);
    static Mesh* CreateFromFile(std::string const& name, std::string const& file);

    std::vector<Vertex> Vertices;
    std::vector<uint32_t> Indices;

private:
    std::string Name;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};
