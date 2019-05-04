#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm/gtc/constants.hpp>

Mesh::Mesh(std::string const& name) :
    Name(name)
{
    fprintf(stdout, "Created mesh '%s'!\n", Name.c_str());
}

Mesh::~Mesh()
{
    fprintf(stdout, "Deleted mesh '%s'!\n", Name.c_str());
}

Mesh* Mesh::CreateQuad(std::string const& name, float width, float height)
{
    Mesh* mesh = new Mesh(name);

    glm::vec2 uv00 = glm::vec2(0.f, 0.f);
    glm::vec2 uv10 = glm::vec2(1.f, 0.f);
    glm::vec2 uv01 = glm::vec2(0.f, 1.f);
    glm::vec2 uv11 = glm::vec2(1.f, 1.f);

    mesh->Vertices.push_back(Vertex(-width / 2.f, -height / 2.f, 0.f, 0.f, 0.f, 1.f, uv00));
    mesh->Vertices.push_back(Vertex(-width / 2.f,  height / 2.f, 0.f, 0.f, 0.f, 1.f, uv01));
    mesh->Vertices.push_back(Vertex( width / 2.f, -height / 2.f, 0.f, 0.f, 0.f, 1.f, uv10));
    mesh->Vertices.push_back(Vertex( width / 2.f,  height / 2.f, 0.f, 0.f, 0.f, 1.f, uv11));

    return mesh;
}

Mesh* Mesh::CreateCube(std::string const& name, float width, float height, float depth)
{
    Mesh* mesh = new Mesh(name);

    glm::vec2 uv00 = glm::vec2(0.f, 0.f);
    glm::vec2 uv10 = glm::vec2(1.f, 0.f);
    glm::vec2 uv01 = glm::vec2(0.f, 1.f);
    glm::vec2 uv11 = glm::vec2(1.f, 1.f);

    // Vorne | Dreieck Rechts
    mesh->Vertices.push_back(Vertex( width / 2.f, -height / 2.f,  depth / 2.f, 0.f, 0.f, 1.f, uv10)); // Rechts | Unten | Vorne   0
    mesh->Vertices.push_back(Vertex( width / 2.f,  height / 2.f,  depth / 2.f, 0.f, 0.f, 1.f, uv11)); // Rechts | Oben  | Vorne   1
    mesh->Vertices.push_back(Vertex(-width / 2.f, -height / 2.f,  depth / 2.f, 0.f, 0.f, 1.f, uv00)); // Links  | Unten | Vorne   2

    // Vorne | Dreieck Links
    // Vertex 2
    // Vertex 1
    mesh->Vertices.push_back(Vertex(-width / 2.f,  height / 2.f,  depth / 2.f, 0.f, 0.f, 1.f, uv01)); // Links  | Oben  | Vorne   3

    // Links | Dreieck Rechts
    mesh->Vertices.push_back(Vertex(-width / 2.f, -height / 2.f,  depth / 2.f, -1.f, 0.f, 0.f, uv10)); // Links  | Unten | Vorne   4
    mesh->Vertices.push_back(Vertex(-width / 2.f,  height / 2.f,  depth / 2.f, -1.f, 0.f, 0.f, uv11)); // Links  | Oben  | Vorne   5
    mesh->Vertices.push_back(Vertex(-width / 2.f, -height / 2.f, -depth / 2.f, -1.f, 0.f, 0.f, uv00)); // Links  | Unten | Hinten  6

    // Links | Dreieck Links
    // Vertex 6
    // Vertex 5
    mesh->Vertices.push_back(Vertex(-width / 2.f,  height / 2.f, -depth / 2.f, -1.f, 0.f, 0.f, uv01)); // Links  | Oben  | Hinten  7

    // Hinten | Dreieck Rechts
    mesh->Vertices.push_back(Vertex(-width / 2.f, -height / 2.f, -depth / 2.f, 0.f, 0.f, -1.f, uv00)); // Links  | Unten | Hinten  8
    mesh->Vertices.push_back(Vertex(-width / 2.f,  height / 2.f, -depth / 2.f, 0.f, 0.f, -1.f, uv01)); // Links  | Oben  | Hinten  9
    mesh->Vertices.push_back(Vertex( width / 2.f, -height / 2.f, -depth / 2.f, 0.f, 0.f, -1.f, uv10)); // Rechts | Unten | Hinten  10

    // Hinten | Dreieck Links
    // Vertex 10
    // Vertex 9
    mesh->Vertices.push_back(Vertex( width / 2.f,  height / 2.f, -depth / 2.f, 0.f, 0.f, -1.f, uv11)); // Rechts | Oben  | Hinten  11

    // Rechts | Dreieck Rechts
    mesh->Vertices.push_back(Vertex( width / 2.f, -height / 2.f, -depth / 2.f, 1.f, 0.f, 0.f, uv10)); // Rechts | Unten | Hinten  12
    mesh->Vertices.push_back(Vertex( width / 2.f,  height / 2.f, -depth / 2.f, 1.f, 0.f, 0.f, uv11)); // Rechts | Oben  | Hinten  13
    mesh->Vertices.push_back(Vertex( width / 2.f, -height / 2.f,  depth / 2.f, 1.f, 0.f, 0.f, uv00)); // Rechts | Unten | Vorne   14

    // Rechts | Dreieck Links
    // Vertex 14
    // Vertex 13
    mesh->Vertices.push_back(Vertex( width / 2.f,  height / 2.f,  depth / 2.f, 1.f, 0.f, 0.f, uv01)); // Rechts | Oben  | Vorne   15

    // Oben | Dreieck Rechts
    mesh->Vertices.push_back(Vertex( width / 2.f,  height / 2.f,  depth / 2.f, 0.f, 1.f, 0.f, uv10)); // Rechts | Oben  | Vorne   16
    mesh->Vertices.push_back(Vertex( width / 2.f,  height / 2.f, -depth / 2.f, 0.f, 1.f, 0.f, uv11)); // Rechts | Oben  | Hinten  17
    mesh->Vertices.push_back(Vertex(-width / 2.f,  height / 2.f,  depth / 2.f, 0.f, 1.f, 0.f, uv00)); // Links  | Oben  | Vorne   18

    // Oben | Dreieck Links
    // Vertex 18
    // Vertex 17
    mesh->Vertices.push_back(Vertex(-width / 2.f,  height / 2.f, -depth / 2.f, 0.f, 1.f, 0.f, uv01)); // Links  | Oben  | Hinten  19

    // Unten | Dreieck Rechts
    mesh->Vertices.push_back(Vertex(-width / 2.f, -height / 2.f,  depth / 2.f, 0.f, -1.f, 0.f, uv00)); // Links  | Unten | Vorne   20
    mesh->Vertices.push_back(Vertex(-width / 2.f, -height / 2.f, -depth / 2.f, 0.f, -1.f, 0.f, uv01)); // Links  | Unten | Hinten  21
    mesh->Vertices.push_back(Vertex( width / 2.f, -height / 2.f,  depth / 2.f, 0.f, -1.f, 0.f, uv10)); // Rechts | Unten | Vorne   22

    // Unten | Dreieck Links
    // Vertex 22
    // Vertex 21
    mesh->Vertices.push_back(Vertex( width / 2.f, -height / 2.f, -depth / 2.f, 0.f, -1.f, 0.f, uv11)); // Rechts | Unten | Hinten  23


    mesh->Indices.push_back(0);  mesh->Indices.push_back(1);  mesh->Indices.push_back(2);  // Vorne  | Dreieck Rechts
    mesh->Indices.push_back(2);  mesh->Indices.push_back(1);  mesh->Indices.push_back(3);  // Vorne  | Dreieck Links
    mesh->Indices.push_back(4);  mesh->Indices.push_back(5);  mesh->Indices.push_back(6);  // Links  | Dreieck Rechts
    mesh->Indices.push_back(6);  mesh->Indices.push_back(5);  mesh->Indices.push_back(7);  // Links  | Dreieck Links
    mesh->Indices.push_back(8);  mesh->Indices.push_back(9);  mesh->Indices.push_back(10); // Hinten | Dreieck Rechts
    mesh->Indices.push_back(10); mesh->Indices.push_back(9);  mesh->Indices.push_back(11); // Hinten | Dreieck Links
    mesh->Indices.push_back(12); mesh->Indices.push_back(13); mesh->Indices.push_back(14); // Rechts | Dreieck Rechts
    mesh->Indices.push_back(14); mesh->Indices.push_back(13); mesh->Indices.push_back(15); // Rechts | Dreieck Links
    mesh->Indices.push_back(16); mesh->Indices.push_back(17); mesh->Indices.push_back(18); // Oben   | Dreieck Rechts
    mesh->Indices.push_back(18); mesh->Indices.push_back(17); mesh->Indices.push_back(19); // Oben   | Dreieck Links
    mesh->Indices.push_back(20); mesh->Indices.push_back(21); mesh->Indices.push_back(22); // Unten  | Dreieck Rechts
    mesh->Indices.push_back(22); mesh->Indices.push_back(21); mesh->Indices.push_back(23); // Unten  | Dreieck Links

    return mesh;
}

Mesh* Mesh::CreateCylinder(std::string const& name, uint32_t segments, float radius, float height)
{
    Mesh* mesh = new Mesh(name);

    float segmentAngle = (glm::pi<float>() * 2.f) / segments;

    // Generate bottom and top circle vertices
    for (uint32_t i = 0; i < 2; i++)
    {
        // Bottom circle first
        float y = -0.5f * height + i * height;

        for (uint32_t j = 0; j <= segments; j++)
        {
            glm::vec3 position = glm::vec3(radius * cos(j * segmentAngle), y, radius * sin(j * segmentAngle));
            glm::vec3 normal = glm::normalize(glm::vec3(position.x, 0.f, position.z));
            glm::vec2 uv = glm::vec2((float)j / segments, (i == 0) ? 0.f : 1.f);
            mesh->Vertices.push_back(Vertex(position, normal, uv));
        }
    }

    // Generate side indices
    for (uint32_t i = 0, verticesPerCircle = segments + 1; i < segments; i++)
    {
        mesh->Indices.push_back(i);
        mesh->Indices.push_back(verticesPerCircle + i);
        mesh->Indices.push_back(verticesPerCircle + i + 1);

        mesh->Indices.push_back(i);
        mesh->Indices.push_back(verticesPerCircle + i + 1);
        mesh->Indices.push_back(i + 1);
    }

    uint32_t bottomCircleVertexIndex = mesh->Vertices.size();

    // Generate bottom and top circle vertices again...
    for (uint32_t i = 0; i < 2; i++)
    {
        // Bottom circle first
        float y = -0.5f * height + i * height;

        for (uint32_t j = 0; j <= segments; j++)
        {
            glm::vec3 position = glm::vec3(radius * cos(j * segmentAngle), y, radius * sin(j * segmentAngle));
            glm::vec3 normal = (i == 0) ? glm::vec3(0.f, -1.f, 0.f) : glm::vec3(0.f, 1.f, 0.f);
            glm::vec2 uv = glm::vec2(cos(j * segmentAngle) / 2.f + 0.5f, sin(j * segmentAngle) / 2.f + 0.5f);
            mesh->Vertices.push_back(Vertex(position, normal, uv));
        }
    }

    mesh->Vertices.push_back(Vertex(0.f, -0.5f * height, 0.f, 0.f, -1.f, 0.f, glm::vec2(0.5f, 0.5f)));

    for (uint32_t i = bottomCircleVertexIndex, centerVertexIndex = mesh->Vertices.size() - 1, to = bottomCircleVertexIndex + segments; i < to; i++)
    {
        mesh->Indices.push_back(centerVertexIndex);
        mesh->Indices.push_back(i);
        mesh->Indices.push_back(i + 1);
    }

    mesh->Vertices.push_back(Vertex(0.f, 0.5f * height, 0.f, 0.f, 1.f, 0.f, glm::vec2(0.5f, 0.5f)));

    for (uint32_t i = bottomCircleVertexIndex, centerVertexIndex = mesh->Vertices.size() - 1, verticesPerCircle = segments + 1, to = bottomCircleVertexIndex + segments; i < to; i++)
    {
        mesh->Indices.push_back(centerVertexIndex);
        mesh->Indices.push_back(verticesPerCircle + i + 1);
        mesh->Indices.push_back(verticesPerCircle + i);
    }

    return mesh;
}

Mesh* Mesh::CreateSphere(std::string const& name, uint32_t segmentsLongitude, uint32_t segmentsLatitude, float radius)
{
    Mesh* mesh = new Mesh(name);

    float latitudeSegmentAngle = glm::pi<float>() / segmentsLatitude;
    float longitudeSegmentAngle = glm::two_pi<float>() / segmentsLongitude;

    for (uint32_t i = 0; i < segmentsLongitude; i++)
    {
        float currentLongitudeAngle = i * longitudeSegmentAngle;

        mesh->Vertices.push_back(Vertex(0.f, radius, 0.f, 0.f, 1.f, 0.f, glm::vec2(currentLongitudeAngle / glm::two_pi<float>(), 0.f)));
    }

    for (uint32_t i = 1; i < segmentsLatitude; i++)
    {
        float currentLatitudeAngle = i * latitudeSegmentAngle;

        for (uint32_t j = 0; j <= segmentsLongitude; j++)
        {
            float currentLongitudeAngle = j * longitudeSegmentAngle;

            glm::vec3 position = glm::vec3
            (
                radius * sin(currentLatitudeAngle) * cos(currentLongitudeAngle),
                radius * cos(currentLatitudeAngle),
                radius * sin(currentLatitudeAngle) * sin(currentLongitudeAngle)
            );
            glm::vec3 normal = glm::normalize(position);
            glm::vec2 uv = glm::vec2(currentLongitudeAngle / glm::two_pi<float>(), currentLatitudeAngle / glm::pi<float>());

            mesh->Vertices.push_back(Vertex(position, normal, uv));
        }
    }

    for (uint32_t i = 0; i < segmentsLongitude; i++)
    {
        float currentLongitudeAngle = i * longitudeSegmentAngle;

        mesh->Vertices.push_back(Vertex(0.f, -radius, 0.f, 0.f, -1.f, 0.f, glm::vec2(currentLongitudeAngle / glm::two_pi<float>(), 1.f)));
    }

    for (uint32_t i = 1; i <= segmentsLongitude; i++)
    {
        mesh->Indices.push_back(i - 1);
        mesh->Indices.push_back(segmentsLongitude + i);
        mesh->Indices.push_back(segmentsLongitude + i - 1);
    }

    for (uint32_t i = 0, verticesPerCircle = segmentsLongitude + 1; i < (segmentsLatitude - 2); i++)
    {
        for (uint32_t j = 0; j < segmentsLongitude; j++)
        {
            mesh->Indices.push_back(i * verticesPerCircle       + j +     segmentsLongitude /* Circle Start Offset */);
            mesh->Indices.push_back(i * verticesPerCircle       + j + 1 + segmentsLongitude /* Circle Start Offset */);
            mesh->Indices.push_back((i + 1) * verticesPerCircle + j +     segmentsLongitude /* Circle Start Offset */);
            
            mesh->Indices.push_back((i + 1) * verticesPerCircle + j +     segmentsLongitude /* Circle Start Offset */);
            mesh->Indices.push_back(i * verticesPerCircle       + j + 1 + segmentsLongitude /* Circle Start Offset */);
            mesh->Indices.push_back((i + 1) * verticesPerCircle + j + 1 + segmentsLongitude /* Circle Start Offset */);
        }
    }

    for (uint32_t i = 0, bottomCenterVertexIndex = mesh->Vertices.size() - segmentsLongitude, startIndex = mesh->Vertices.size() - 2 * segmentsLongitude - 1; i < segmentsLongitude; i++, bottomCenterVertexIndex++)
    {
        mesh->Indices.push_back(bottomCenterVertexIndex);
        mesh->Indices.push_back(startIndex + i);
        mesh->Indices.push_back(startIndex + i + 1);
    }

    return mesh;
}

Mesh* Mesh::CreateTorus(std::string const& name, uint32_t tubeSegments, uint32_t crossSectionSegments, float r1, float r2)
{
    Mesh* mesh = new Mesh(name);

    for (uint32_t i = 0; i < tubeSegments; i++)
    {
        uint32_t segment = i;

        float segmentAngle = (segment / (float)tubeSegments) * glm::two_pi<float>();
        glm::vec3 segmentPosition = glm::vec3
        (
            r1 * cos(segmentAngle),
            0.f,
            r1 * sin(segmentAngle)
        );

        for (uint32_t j = 0; j < crossSectionSegments; j++)
        {
            uint32_t side = j;

            float sideAngle = (side / (float)crossSectionSegments) * glm::two_pi<float>();
            glm::vec3 sidePosition = glm::angleAxis(-segmentAngle, glm::vec3(0.f, 1.f, 0.f)) * glm::vec3
            (
                r2 * sin(sideAngle),
                r2 * cos(sideAngle),
                0.f
            );
            glm::vec3 normal = glm::normalize(sidePosition);

            mesh->Vertices.push_back(Vertex(segmentPosition + sidePosition, normal));
        }
    }

    for (uint32_t i = 0; i < tubeSegments; i++)
    {
        for (uint32_t j = 0; j < crossSectionSegments; j++)
        {
            uint32_t curr = i * crossSectionSegments + j;
            uint32_t next = (i + 1) * crossSectionSegments + j;

            // Connect last tube segment to first one
            if (i == (tubeSegments - 1))
            {
                next = j;
            }

            // Connect last cross section segment to first one
            if (j == (crossSectionSegments - 1))
            {
                mesh->Indices.push_back(curr);
                mesh->Indices.push_back(next);
                mesh->Indices.push_back(next - j + 1);

                mesh->Indices.push_back(curr);
                mesh->Indices.push_back(next - j + 1);
                mesh->Indices.push_back(curr - j + 1);
            }
            else
            {
                mesh->Indices.push_back(curr);
                mesh->Indices.push_back(next);
                mesh->Indices.push_back(next + 1);

                mesh->Indices.push_back(curr);
                mesh->Indices.push_back(next + 1);
                mesh->Indices.push_back(curr + 1);
            }
        }
    }

    return mesh;
}

// Source: http://ogldev.atspace.co.uk/www/tutorial22/tutorial22.html
Mesh* Mesh::CreateFromFile(std::string const& name, std::string const& file)
{
    const static aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file
    const aiScene* aiScene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    // Check if import failed
    if (!aiScene)
    {
        return nullptr;
    }

    // Check if there is at least one mesh
    if (aiScene->mNumMeshes < 1)
    {
        return nullptr;
    }

    Mesh* mesh = new Mesh(name);

    for (uint32_t j = 0, indexOffset = 0; j < aiScene->mNumMeshes; j++)
    {
        const aiMesh* aiMesh = aiScene->mMeshes[j];

        if (!aiMesh)
        {
            continue;
        }

        const aiMaterial* aiMaterial = (aiMesh->mMaterialIndex >= 0 && aiMesh->mMaterialIndex < aiScene->mNumMaterials) ?
            aiScene->mMaterials[aiMesh->mMaterialIndex] : nullptr;

        aiColor4D color = aiColor4D(1.f, 1.f, 1.f, 1.f);
        if (aiMaterial != nullptr)
        {
            aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        }

        for (uint32_t i = 0; i < aiMesh->mNumVertices; i++)
        {
            const aiVector3D* pos = &(aiMesh->mVertices[i]);
            const aiVector3D* normal = aiMesh->HasNormals() ? &(aiMesh->mNormals[i]) : &Zero3D;
            const aiVector3D* uv = aiMesh->HasTextureCoords(0) ? &(aiMesh->mTextureCoords[0][i]) : &Zero3D;

            mesh->Vertices.push_back(Vertex(pos->x, pos->y, pos->z, normal->x, normal->y, normal->z, uv->x, uv->y, color.r, color.g, color.b));
        }

        for (uint32_t i = 0; i < aiMesh->mNumFaces; i++)
        {
            const aiFace& aiFace = aiMesh->mFaces[i];
            assert(aiFace.mNumIndices == 3);

            mesh->Indices.push_back(indexOffset + aiFace.mIndices[0]);
            mesh->Indices.push_back(indexOffset + aiFace.mIndices[1]);
            mesh->Indices.push_back(indexOffset + aiFace.mIndices[2]);
        }

        indexOffset += aiMesh->mNumFaces * 3;
    }

    return mesh;
}
