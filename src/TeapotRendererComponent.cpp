#include "TeapotRendererComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "ShaderProgram.h"
#include "Utils.h"

TeapotRendererComponent::TeapotRendererComponent(GameObject* owner) :
    Component::Component(owner),
    camera(nullptr),
    transform(nullptr),
    shaderProgram(nullptr),
    color(0.f, 0.f, 0.f)
{
    fprintf(stdout, "Attached teapot renderer component to game object '%s'!\n", GetOwner()->GetName().c_str());
}

void TeapotRendererComponent::OnStart()
{
    transform = GetOwner()->GetComponent<TransformComponent>();
}

TeapotRendererComponent::~TeapotRendererComponent()
{
    fprintf(stdout, "Deleted teapot renderer component from game object '%s'!\n", GetOwner()->GetName().c_str());
}

void TeapotRendererComponent::Render()
{
    if (shaderProgram)
    {
        shaderProgram->Use();

        shaderProgram->SetUniform3fv(0 /* Color */, color);
        shaderProgram->SetUniformMatrix4fv(1 /* Model Matrix */, transform ? transform->GetModelMatrix() : glm::mat4(1.f));
        shaderProgram->SetUniformMatrix4fv(2 /* View Projection Matrix */, camera ? camera->GetViewProjectionMatrix() : glm::mat4(1.f));

        drawTeapot();
    }
}
