#include "vpch.h"
#include "Nut.h"
#include "Physics/Raycast.h"
#include "Bolt.h"

using namespace DirectX;

void Nut::Create()
{
	__super::Create();

	ignoreRotationValidCheck = true;

	SetMeshFilename("nut.vmesh");
}

void Nut::OnLinkRotateLeft()
{
	__super::OnLinkRotateLeft();

	constexpr float moveIncrement = 0.2f;
	nextPos += GetUpVectorV() * moveIncrement;
}

void Nut::OnLinkRotateRight()
{
	__super::OnLinkRotateRight();

	CheckIfConnectedToBolt(-GetUpVectorV());
}

//@Todo: this logic still needs work. Come back to it after more puzzles are done.
void Nut::CheckIfConnectedToBolt(const DirectX::XMVECTOR moveDirection)
{
	HitResult hit(this);
	if (Physics::Raycast(hit, GetPositionV(), moveDirection, 10.f))
	{
		if (hit.GetHitActorAs<Bolt>())
		{
			constexpr float moveIncrement = 0.2f;
			nextPos += moveDirection * moveIncrement;
		}
	}
}
