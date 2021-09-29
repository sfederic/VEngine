#include "BoxTriggerComponent.h"
#include "Components/MeshComponent.h"

BoxTriggerComponent::BoxTriggerComponent()
{
}

void BoxTriggerComponent::Tick(double deltaTime)
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
	boundingBox.Center = transform.position;
	boundingBox.Extents = transform.scale;

	if (boundingBox.Contains(point))
	{
		return true;
	}

	return false;
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
