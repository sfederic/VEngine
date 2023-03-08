#include "vpch.h"
#include "BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"
#include "Actors/Game/Player.h"
#include "Physics/Raycast.h"

BoxTriggerComponent::BoxTriggerComponent()
{
	layer = CollisionLayers::Editor;

	//Keep in mind with triggers that you want them just slightly smaller than 1x1x1 on the grid
	//so you're not always getting Intersects results from the bounding volumes.
	boundingBox.Extents = XMFLOAT3(0.45f, 0.45f, 0.45f);
}

Properties BoxTriggerComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = "BoxTriggerComponent";
	props.Add("Extents", &boundingBox.Extents);
	props.Add("Offset", &boundingBox.Center);
	return props;
}

bool BoxTriggerComponent::Contains(XMVECTOR point)
{
	BoundingOrientedBox bb = VMath::GetBoundingBoxInWorld(this);
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
	targetActor = (Actor*)Player::system.GetFirstActor();
}

XMVECTOR BoxTriggerComponent::GetRandomPointInTrigger()
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, GetWorldPositionV());

	float lowX = pos.x - boundingBox.Extents.x;
	float highX = pos.x + boundingBox.Extents.x;
	
	float lowY = pos.y - boundingBox.Extents.y;
	float highY = pos.y + boundingBox.Extents.y;

	float lowZ = pos.z - boundingBox.Extents.z;
	float highZ = pos.z + boundingBox.Extents.z;

	XMFLOAT3 result{};
	result.x = VMath::RandomRange(lowX, highX);
	result.y = VMath::RandomRange(lowY, highY);
	result.z = VMath::RandomRange(lowZ, highZ);

	return XMLoadFloat3(&pos);
}

bool BoxTriggerComponent::IntersectsWithAnyBoundingBoxInWorld()
{
	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		if (boundingBox.Intersects(mesh->GetBoundingBox()))
		{
			return true;
		}
	}

	return false;
}

void BoxTriggerComponent::SetExtents(float x, float y, float z)
{
	boundingBox.Extents = XMFLOAT3(x, y, z);
}

XMFLOAT3 BoxTriggerComponent::GetExtents()
{
	return boundingBox.Extents;
}

bool BoxTriggerComponent::QuickInPlaceBoxCast(HitResult& hitResult, bool drawDebug)
{
	return SimpleBoxCast(GetWorldPositionV(), boundingBox.Extents, hitResult, drawDebug);
}
