#include "vpch.h"
#include "WindowGridActor.h"
#include "Player.h"

void WindowGridActor::Create()
{
	__super::Create();

	SetMeshFilename("plane.vmesh");
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
