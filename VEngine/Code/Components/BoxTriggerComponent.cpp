#include "BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "VMath.h"

BoxTriggerComponent::BoxTriggerComponent()
{
	layer = CollisionLayers::Editor;

	//Keep in mind with triggers that you want them just slightly smaller than 1x1x1 on the grid
	//so you're not always getting Intersects results from the bounding volumes.
	boundingBox.Extents = XMFLOAT3(0.9f, 0.9f, 0.9f);
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
		bool result = Contains(targetPos);
		return result;
	}

	return false;
}
