#include "vpch.h"
#include "Nut.h"
#include "Physics/Raycast.h"
#include "Bolt.h"

void Nut::Create()
{
	__super::Create();

	ignoreRotationValidCheck = true;

	SetMeshFilename("nut.vmesh");
}

void Nut::OnLinkRotateLeft()
{
	__super::OnLinkRotateLeft();

	CheckIfConnectedToBolt(-GetUpVectorV());
}

void Nut::OnLinkRotateRight()
{
	__super::OnLinkRotateRight();

	CheckIfConnectedToBolt(GetUpVectorV());
}

void Nut::CheckIfConnectedToBolt(const XMVECTOR moveDirection)
{
	HitResult hit(this);
	if (SimpleBoxCast(GetPositionV(), XMFLOAT3(0.5f, 0.5f, 0.5f), hit, false, false))
	{
		for (auto actor : hit.hitActors)
		{
			auto bolt = dynamic_cast<Bolt*>(actor);
			if (bolt)
			{
				constexpr float moveIncrement = 0.2f;
				nextPos += moveDirection * moveIncrement;
			}
		}
	}
}
