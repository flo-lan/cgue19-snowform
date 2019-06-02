#include "ParticleSystemComponentFactory.h"
#include "ParticleSystemComponent.h"
#include "AssetManager.h"
#include "GameObject.h"
#include "Material.h"
#include "tinyxml2.h"
#include "StringUtils.h"

ParticleSystemComponentFactory::ParticleSystemComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void ParticleSystemComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    ParticleSystemComponent* particleSystem = gameObject->AttachComponent<ParticleSystemComponent>();

    if (element->Attribute("enabled"))
    {
        particleSystem->SetEnabled(std::string(element->Attribute("enabled")) == "true");
    }

    if (element->Attribute("particleCount"))
    {
        std::string particleCountValue = std::string(element->Attribute("particleCount"));
        particleSystem->SetParticleCount(std::atoi(particleCountValue.c_str()));
    }

    if (element->Attribute("particleMaterial"))
    {
        std::string particleMaterialName = std::string(element->Attribute("particleMaterial"));

        if (Material* material = sAssetManager.GetMaterial(particleMaterialName))
        {
            particleSystem->SetParticleMaterial(material);
        }
        else
        {
            fprintf(stderr, "Could not find particle material '%s' for particle system component of game object '%s'!\n", particleMaterialName.c_str(), gameObject->GetName().c_str());
        }
    }

    if (element->Attribute("particleMesh"))
    {
        std::string particleMeshName = std::string(element->Attribute("particleMesh"));

        if (Mesh* mesh = sAssetManager.GetMesh(particleMeshName))
        {
            particleSystem->SetParticleMesh(mesh);
        }
        else
        {
            fprintf(stderr, "Could not find particle mesh '%s' for particle system component for game object '%s'!\n", particleMeshName.c_str(), gameObject->GetName().c_str());
        }
    }

    if (element->Attribute("minRelativeEmitPosition"))
    {
        std::string value = std::string(element->Attribute("minRelativeEmitPosition"));
        std::vector<std::string> valueSplit = split(value, ' ');

        glm::vec3 minRelativeEmitPosition = glm::vec3(0.f, 0.f, 0.f);

        switch (valueSplit.size())
        {
            case 3:
                minRelativeEmitPosition.z = (float)::atof(valueSplit[2].c_str());
                // No break
            case 2:
                minRelativeEmitPosition.y = (float)::atof(valueSplit[1].c_str());
                // No break
            case 1:
                minRelativeEmitPosition.x = (float)::atof(valueSplit[0].c_str());
                break;
            default:
                fprintf(stderr, "Could not parse min relative emit position for particle system component of game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        particleSystem->SetMinRelativeEmitPosition(minRelativeEmitPosition);
    }

    if (element->Attribute("maxRelativeEmitPosition"))
    {
        std::string value = std::string(element->Attribute("maxRelativeEmitPosition"));
        std::vector<std::string> valueSplit = split(value, ' ');

        glm::vec3 maxRelativeEmitPosition = glm::vec3(0.f, 0.f, 0.f);

        switch (valueSplit.size())
        {
            case 3:
                maxRelativeEmitPosition.z = (float)::atof(valueSplit[2].c_str());
                // No break
            case 2:
                maxRelativeEmitPosition.y = (float)::atof(valueSplit[1].c_str());
                // No break
            case 1:
                maxRelativeEmitPosition.x = (float)::atof(valueSplit[0].c_str());
                break;
            default:
                fprintf(stderr, "Could not parse max relative emit position for particle system component of game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        particleSystem->SetMaxRelativeEmitPosition(maxRelativeEmitPosition);
    }

    if (element->Attribute("minParticleVelocity"))
    {
        std::string value = std::string(element->Attribute("minParticleVelocity"));
        std::vector<std::string> valueSplit = split(value, ' ');

        glm::vec3 minParticleVelocity = glm::vec3(0.f, 0.f, 0.f);

        switch (valueSplit.size())
        {
            case 3:
                minParticleVelocity.z = (float)::atof(valueSplit[2].c_str());
                // No break
            case 2:
                minParticleVelocity.y = (float)::atof(valueSplit[1].c_str());
                // No break
            case 1:
                minParticleVelocity.x = (float)::atof(valueSplit[0].c_str());
                break;
            default:
                fprintf(stderr, "Could not parse min particle velocity for particle system component of game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        particleSystem->SetMinParticleVelocity(minParticleVelocity);
    }

    if (element->Attribute("maxParticleVelocity"))
    {
        std::string value = std::string(element->Attribute("maxParticleVelocity"));
        std::vector<std::string> valueSplit = split(value, ' ');

        glm::vec3 maxParticleVelocity = glm::vec3(0.f, 0.f, 0.f);

        switch (valueSplit.size())
        {
            case 3:
                maxParticleVelocity.z = (float)::atof(valueSplit[2].c_str());
                // No break
            case 2:
                maxParticleVelocity.y = (float)::atof(valueSplit[1].c_str());
                // No break
            case 1:
                maxParticleVelocity.x = (float)::atof(valueSplit[0].c_str());
                break;
            default:
                fprintf(stderr, "Could not parse max particle velocity for particle system component of game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        particleSystem->SetMaxParticleVelocity(maxParticleVelocity);
    }

    if (element->Attribute("minParticleSize"))
    {
        std::string value = std::string(element->Attribute("minParticleSize"));
        particleSystem->SetMinParticleSize(std::strtof(value.c_str(), 0));
    }

    if (element->Attribute("maxParticleSize"))
    {
        std::string value = std::string(element->Attribute("maxParticleSize"));
        particleSystem->SetMaxParticleSize(std::strtof(value.c_str(), 0));
    }

    if (element->Attribute("minParticleLifetime"))
    {
        std::string value = std::string(element->Attribute("minParticleLifetime"));
        particleSystem->SetMinParticleLifetime(std::strtof(value.c_str(), 0));
    }

    if (element->Attribute("maxParticleLifetime"))
    {
        std::string value = std::string(element->Attribute("maxParticleLifetime"));
        particleSystem->SetMaxParticleLifetime(std::strtof(value.c_str(), 0));
    }
}
