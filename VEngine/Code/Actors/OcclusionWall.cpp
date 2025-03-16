
#include "OcclusionWall.h"
import Components.MeshComponent;
#include "Core/Camera.h"
import Physics.Raycast;
import Actors.Game.Player;

OcclusionWall::OcclusionWall()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	SetRootComponent(mesh);
	mesh->SetMeshFilename("cube.vmesh");
}

void OcclusionWall::Tick(float)
{
	auto& cam = Camera::GetActiveCamera();

	HitResult hit;
	auto player = Player::system.GetOnlyActor();
	const auto cameraToPlayerDir = XMVector3Normalize(cam.GetWorldPositionV() - player->GetPositionV());
	const auto start = player->GetPositionV() + cameraToPlayerDir;
	if (Physics::OrientedBoxCast(hit, start, cam.GetWorldPositionV(), XMFLOAT2(0.25f, 0.25f), false, false))
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
