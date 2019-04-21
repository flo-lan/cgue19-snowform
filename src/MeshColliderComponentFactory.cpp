#include "MeshColliderComponentFactory.h"
#include "MeshColliderComponent.h"
#include "PhysicsEngine.h"
#include "AssetManager.h"
#include "StringUtils.h"
#include "GameObject.h"
#include "tinyxml2.h"
#include "PxMaterial.h"

MeshColliderComponentFactory::MeshColliderComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void MeshColliderComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    MeshColliderComponent* meshCollider = gameObject->AttachComponent<MeshColliderComponent>();

    if (element->Attribute("physicsMaterial"))
    {
        std::string physicsMaterialName = std::string(element->Attribute("physicsMaterial"));

        if (physx::PxMaterial* pxMaterial = sPhysicsEngine.GetPxMaterial(physicsMaterialName))
        {
            meshCollider->SetPxMaterial(pxMaterial);
        }
        else
        {
            fprintf(stderr, "Could not find PhysX material '%s' for mesh collider component of game object '%s'!\n", physicsMaterialName.c_str(), gameObject->GetName().c_str());
        }
    }

    if (element->Attribute("collisionMesh"))
    {
        std::string collisionMeshName = std::string(element->Attribute("collisionMesh"));

        if (Mesh* mesh = sAssetManager.GetMesh(collisionMeshName))
        {
            meshCollider->SetMesh(mesh);
        }
        else
        {
            fprintf(stderr, "Could not find collision mesh '%s' for mesh collider component game object '%s'!\n", collisionMeshName.c_str(), gameObject->GetName().c_str());
        }
    }

    if (element->Attribute("offset"))
    {
        std::string offsetString = std::string(element->Attribute("offset"));
        std::vector<std::string> offsetStringSplit = split(offsetString, ' ');

        glm::vec3 offset = glm::vec3(0.f, 0.f, 0.f);

        switch (offsetStringSplit.size())
        {
        case 3:
            offset.z = (float)::atof(offsetStringSplit[2].c_str());
            // No break
        case 2:
            offset.y = (float)::atof(offsetStringSplit[1].c_str());
            // No break
        case 1:
            offset.x = (float)::atof(offsetStringSplit[0].c_str());
            break;
        default:
            fprintf(stderr, "Could not parse offset for mesh collider component of game object '%s'!\n", gameObject->GetName().c_str());
            break;
        }

        meshCollider->SetOffset(offset);
    }
}
