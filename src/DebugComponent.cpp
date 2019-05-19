#include "DebugComponent.h"
#include "GameObject.h"
#include "Collision.h"
#include "ColliderComponent.h"

DebugComponent::DebugComponent(GameObject* owner) :
    Component::Component(owner)
{
}

DebugComponent::~DebugComponent()
{
}

void DebugComponent::OnAwake()
{
    fprintf(stdout, "[DebugComponent][%s] OnAwake\n", GetOwner()->GetName().c_str());
}

void DebugComponent::OnStart()
{
    fprintf(stdout, "[DebugComponent][%s] OnStart\n", GetOwner()->GetName().c_str());
}

void DebugComponent::OnAttachComponent(Component* component)
{
}

void DebugComponent::OnRemoveComponent(Component* component)
{
}

void DebugComponent::OnCollisionEnter(Collision const& collision)
{
    fprintf(stdout, "[DebugComponent][%s] OnCollisionEnter (collision = %s)\n", GetOwner()->GetName().c_str(), collision.GetGameObject()->GetName().c_str());
}

void DebugComponent::OnCollisionExit(Collision const& collision)
{
    fprintf(stdout, "[DebugComponent][%s] OnCollisionExit (collision = %s)\n", GetOwner()->GetName().c_str(), collision.GetGameObject()->GetName().c_str());
}

void DebugComponent::OnTriggerEnter(ColliderComponent* other)
{
    fprintf(stdout, "[DebugComponent][%s] OnTriggerEnter (other = %s)\n", GetOwner()->GetName().c_str(), other->GetOwner()->GetName().c_str());
}

void DebugComponent::OnTriggerExit(ColliderComponent* other)
{
    fprintf(stdout, "[DebugComponent][%s] OnTriggerExit (other = %s)\n", GetOwner()->GetName().c_str(), other->GetOwner()->GetName().c_str());
}

void DebugComponent::Update()
{
}

void DebugComponent::LateUpdate()
{
}

void DebugComponent::OnDestroy()
{
    fprintf(stdout, "[DebugComponent][%s] OnDestroy\n", GetOwner()->GetName().c_str());
}
