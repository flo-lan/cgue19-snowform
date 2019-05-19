#include "MeshRendererComponentFactory.h"
#include "MeshRendererComponent.h"
#include "AssetManager.h"
#include "Material.h"
#include "Mesh.h"
#include "GameObject.h"
#include "tinyxml2.h"

MeshRendererComponentFactory::MeshRendererComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void MeshRendererComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    MeshRendererComponent* meshRenderer = gameObject->AttachComponent<MeshRendererComponent>();

    if (element->Attribute("enabled"))
    {
        meshRenderer->SetEnabled(std::string(element->Attribute("enabled")) == "true");
    }

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

    for (tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
    {
        std::string childName = std::string(childElement->Name());

        if (childName == "Light")
        {
            if (childElement->Attribute("gameObjectId"))
            {
                std::string gameObjectId = std::string(childElement->Attribute("gameObjectId"));
                meshRenderer->AddInitialLight(gameObjectId);
            }
            else
            {
                fprintf(stderr, "Could not find game object id attribute of light element of mesh renderer component for game object '%s'!\n", gameObject->GetName().c_str());
            }
        }
        else
        {
            fprintf(stderr, "Could not parse XML child '%s' of mesh renderer component for game object '%s'!\n", childName.c_str(), gameObject->GetName().c_str());
        }
    }
}
