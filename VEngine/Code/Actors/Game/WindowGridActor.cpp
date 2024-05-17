#include "vpch.h"
#include "WindowGridActor.h"
#include "Player.h"
#include "Render/RastStates.h"
#include "Render/BlendStates.h"
#include "Components/MeshComponent.h"

void WindowGridActor::Create()
{
	__super::Create();

	SetMeshFilename("plane.vmesh");

	canFall = false;

	_mesh->SetRastState(RastStates::noBackCull);
	_mesh->SetBlendState(BlendStates::Transparent);
	_mesh->SetAlpha(0.5f);
}

//@Todo: some sort of visual effect for the glass breaking. Sprite sheet? Particles? PhysX?

void WindowGridActor::OnLinkMove()
{
	__super::OnLinkMove();

	DeferDestroy();
	Player::system.GetOnlyActor()->ResetLinkedGridActor();
}

void WindowGridActor::OnLinkRotate()
{
	__super::OnLinkRotate();

	DeferDestroy();
	Player::system.GetOnlyActor()->ResetLinkedGridActor();
}
