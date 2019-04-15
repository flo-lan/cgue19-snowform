#include "MeshRendererComponentFactory.h"
#include "MeshRendererComponent.h"
#include "AssetManager.h"
#include "Material.h"
#include "Mesh.h"
#include "GameObject.h"
#include "tinyxml2.h"

MeshRendererComponentFactory::MeshRendererComponentFactory()
{
}

void MeshRendererComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    MeshRendererComponent* meshRenderer = gameObject->AttachComponent<MeshRendererComponent>();

    if (element->Attribute("material"))
    {
        std::string materialName = std::string(element->Attribute("material"));

        if (Material* material = sAssetManager.GetMaterial(materialName))
        {
            meshRenderer->SetMaterial(material);
        }
        else
        {
            fprintf(stderr, "Could not find material '%s' for mesh renderer component of game object '%s'!\n", materialName.c_str(), gameObject->GetName().c_str());
        }
    }

    if (element->Attribute("mesh"))
    {
        std::string meshName = std::string(element->Attribute("mesh"));

        if (Mesh* mesh = sAssetManager.GetMesh(meshName))
        {
            meshRenderer->SetMesh(mesh);
        }
        else
        {
            fprintf(stderr, "Could not find mesh '%s' of mesh renderer component for game object '%s'!\n", meshName.c_str(), gameObject->GetName().c_str());
        }
    }
}
