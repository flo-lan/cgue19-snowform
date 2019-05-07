#include "SceneOptionComponentFactory.h"
#include "Scene.h"
#include "GameObject.h"
#include "tinyxml2.h"

SceneOptionComponentFactory::SceneOptionComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void SceneOptionComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    if (!element->Attribute("key") ||
        !element->Attribute("value"))
    {
        return;
    }

    std::string key = std::string(element->Attribute("key"));
    std::string value = std::string(element->Attribute("value"));

    gameObject->GetScene()->SetOption(key, value);
}
