#include "vpch.h"
#include "OcclusionWall.h"
#include "Components/MeshComponent.h"
#include "Core/Camera.h"
#include "Physics/Raycast.h"
#include "Actors/Game/Player.h"

OcclusionWall::OcclusionWall()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;
	mesh->SetMeshFilename("cube.vmesh");
}

void OcclusionWall::Tick(float deltaTime)
{
	auto& cam = Camera::GetActiveCamera();

	HitResult hit;
	auto player = Player::system.GetOnlyActor();
	if (OrientedBoxCast(hit, player->GetPositionV(), cam.GetWorldPositionV(), XMFLOAT2(0.5f, 0.5f), false, false))
	{
		auto hitActor = hit.FindHitActor(this);
		if (hitActor == this)
		{
			SetVisibility(false);
		}
		else
		{
			SetVisibility(true);
		}
	}
	else
	{
		SetVisibility(true);
	}
}

Properties OcclusionWall::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
