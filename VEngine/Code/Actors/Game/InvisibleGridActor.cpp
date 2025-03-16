
#include "InvisibleGridActor.h"
#include "LookingGlass.h"
import Physics.Raycast;
#include "Core/Camera.h"

void InvisibleGridActor::Create()
{
	__super::Create();
}

void InvisibleGridActor::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	//@Todo: if this idea makes it through, have to come back here and replace the raycast with 
	//a stencil buffer trick to properly render the grid actor through the lens of the looking glass.
	HitResult hit(this);
	if (Physics::Raycast(hit, GetPositionV(), Camera::GetActiveCamera().GetWorldPositionV()))
	{
		auto lookingGlass = hit.GetHitActorAs<LookingGlass>();
		if (lookingGlass)
		{
			SetVisibility(true);
			return;
		}
	}

	SetVisibility(false);
}

Properties InvisibleGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
