#pragma once

#include <glm\glm.hpp>
#include <string>
#include <vector>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 UV;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv) :
        Position(position),
        Normal(normal),
        UV(uv)
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
};

struct Mesh
{
    Mesh(std::string const& name);
    ~Mesh();

    static Mesh* CreateCube(std::string const& name, float width, float height, float depth);
    static Mesh* CreateCylinder(std::string const& name, uint32_t segments, float radius, float height);
    static Mesh* CreateSphere(std::string const& name, uint32_t segmentsLongitude, uint32_t segmentsLatitude, float radius);
    static Mesh* CreateTorus(std::string const& name, uint32_t tubeSegments, uint32_t crossSectionSegments, float r1, float r2);

    std::string Name;
    std::vector<Vertex> Vertices;
    std::vector<uint32_t> Indices;
};
