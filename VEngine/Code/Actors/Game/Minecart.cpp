#include "vpch.h"
#include "Minecart.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"
#include "MinecartTrack.h"
#include "Core/VMath.h"

void Minecart::Create()
{
	mesh->SetMeshFilename("minecart.vmesh");
}

Properties Minecart::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void Minecart::OnLinkMove()
{
	__super::OnLinkMove();

	HitResult hit(this);
	if (Raycast(hit, nextPos, -GetUpVectorV(), 5.f))
	{
		auto track = dynamic_cast<MinecartTrack*>(hit.hitActor);
		if (track)
		{
			const auto forward = GetForwardVectorV();
			const auto trackForward = track->GetForwardVectorV();
			constexpr float epsilon = 0.001f;
			if (VMath::VecEqual(forward, trackForward, epsilon) || 
				VMath::VecEqual(-forward, trackForward, epsilon))
			{
				return;
			}
		}
	}

	nextPos = GetPositionV();
}
