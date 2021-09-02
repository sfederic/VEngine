#include "BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Render/Renderer.h"

BoxTriggerComponent::BoxTriggerComponent()
{
}

void BoxTriggerComponent::Tick(double deltaTime)
{
	XMVECTOR pos = target->GetPositionVector();
	auto rootpos = XMLoadFloat3(&transform.position);
	if (boundingBox.Contains(rootpos))
	{
		throw;
	}
}

Properties BoxTriggerComponent::GetProps()
{
	return Properties();
}

void BoxTriggerComponent::Create()
{
}
