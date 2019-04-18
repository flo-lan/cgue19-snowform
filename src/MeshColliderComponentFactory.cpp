#include "MeshColliderComponentFactory.h"
#include "MeshColliderComponent.h"
#include "PhysicsEngine.h"
#include "AssetManager.h"
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
}
