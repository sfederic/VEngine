#include "BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "VMath.h"

BoxTriggerComponent::BoxTriggerComponent()
{
}

void BoxTriggerComponent::Tick(float deltaTime)
{

}

Properties BoxTriggerComponent::GetProps()
{
	Properties props("BoxTriggerComponent");
	props.Add("Extents", &boundingBox.Extents);
	return props;
}

void BoxTriggerComponent::Create()
{

}

bool BoxTriggerComponent::Contains(XMVECTOR point)
{
	BoundingOrientedBox bb = VMath::GetUpdatedBoundingBox(this);
	return bb.Contains(point);
}

bool BoxTriggerComponent::ContainsTarget()
{
	if (target)
	{
		XMVECTOR targetPos = target->GetPositionVector();
		return Contains(targetPos);
	}

	return false;
}
