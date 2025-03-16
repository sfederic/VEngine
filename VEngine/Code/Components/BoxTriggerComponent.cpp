#include <DirectXMath.h>
#include <DirectXCollision.h>

import Components.BoxTriggerComponent;
import Components.MeshComponent;
import Core.VMath;
import Actors.Game.Player;
import Physics.Raycast;

using namespace DirectX;

BoxTriggerComponent::BoxTriggerComponent()
{
	layer = CollisionLayers::Editor;

	//Keep in mind with triggers that you want them just slightly smaller than 1x1x1 on the grid
	//so you're not always getting Intersects results from the bounding volumes.
	boundingBox.Extents = XMFLOAT3(0.45f, 0.45f, 0.45f);
}

void BoxTriggerComponent::Tick(float)
{
	TargetActorIntersectCallbackLogic();
}

Properties BoxTriggerComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Extents", &boundingBox.Extents);
	props.Add("Offset", &boundingBox.Center);
	return props;
}

bool BoxTriggerComponent::Intersects(const BoundingOrientedBox& boundsInWorldSpaceToCheckAgainst)
{
	const BoundingOrientedBox bbInWorld = GetBoundsInWorldSpace();
	return bbInWorld.Intersects(boundsInWorldSpaceToCheckAgainst);
}

bool BoxTriggerComponent::Contains(XMVECTOR point)
{
	const BoundingOrientedBox bb = GetBoundsInWorldSpace();
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
	XMFLOAT3 pos{};
	XMStoreFloat3(&pos, GetWorldPositionV());

	const float lowX = pos.x - boundingBox.Extents.x;
	const float highX = pos.x + boundingBox.Extents.x;

	const float lowY = pos.y - boundingBox.Extents.y;
	const float highY = pos.y + boundingBox.Extents.y;

	const float lowZ = pos.z - boundingBox.Extents.z;
	const float highZ = pos.z + boundingBox.Extents.z;

	XMFLOAT3 result{};
	result.x = VMath::RandomRange(lowX, highX);
	result.y = VMath::RandomRange(lowY, highY);
	result.z = VMath::RandomRange(lowZ, highZ);

	return XMLoadFloat3(&result);
}

bool BoxTriggerComponent::IntersectsWithAnyBoundingBoxInWorld()
{
	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		const auto thisTriggerBounds = GetBoundsInWorldSpace();
		if (thisTriggerBounds.Intersects(mesh->GetBoundsInWorldSpace()))
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

XMFLOAT3 BoxTriggerComponent::GetExtents() const
{
	return boundingBox.Extents;
}

bool BoxTriggerComponent::QuickInPlaceBoxCast(HitResult& hitResult, bool drawDebug)
{
	return Physics::SimpleBoxCast(GetWorldPositionV(), boundingBox.Extents, hitResult, drawDebug, true);
}

void BoxTriggerComponent::SetTriggerEnterCallback(std::function<void()> callback)
{
	triggerEnterCallback = callback;
}

void BoxTriggerComponent::SetTriggerExitCallback(std::function<void()> callback)
{
	triggerExitCallback = callback;
}

void BoxTriggerComponent::TargetActorIntersectCallbackLogic()
{
	if (targetActor)
	{
		const bool newIntersectingValue = ContainsTarget();
		if (targetActorIntersecting == !newIntersectingValue)
		{
			targetActorIntersecting = newIntersectingValue;

			switch (targetActorIntersecting)
			{
			case true:
				if (triggerEnterCallback) //Note that this is std::function's operator.
				{
					triggerEnterCallback();
				}
				break;
			case false:
				if (triggerExitCallback)
				{
					triggerExitCallback();
				}
				break;
			}
		}
	}
}
