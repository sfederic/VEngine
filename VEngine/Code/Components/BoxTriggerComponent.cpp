#include "vpch.h"
#include "BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "VMath.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Player.h"

BoxTriggerComponent::BoxTriggerComponent()
{
	layer = CollisionLayers::Editor;

	//Keep in mind with triggers that you want them just slightly smaller than 1x1x1 on the grid
	//so you're not always getting Intersects results from the bounding volumes.
	boundingBox.Extents = XMFLOAT3(0.45f, 0.45f, 0.45f);
}

void BoxTriggerComponent::Tick(float deltaTime)
{

}

Properties BoxTriggerComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = "BoxTriggerComponent";
	props.Add("Extents", &boundingBox.Extents);
	props.Add("Offset", &boundingBox.Center);
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
	if (targetActor)
	{
		XMVECTOR targetPos = targetActor->GetPositionV();
		bool result = Contains(targetPos);
		return result;
	}

	return false;
}

void BoxTriggerComponent::SetTargetAsPlayer()
{
	targetActor = (Actor*)GameUtils::GetPlayer();
}
