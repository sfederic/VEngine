#include "vpch.h"
#include "Nut.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"
#include "Bolt.h"

void Nut::Create()
{
	ignoreRotationValidCheck = true;

	mesh->SetMeshFilename("nut.vmesh");
}

void Nut::OnLinkRotate()
{
	CheckIfConnectedToBolt();
}

void Nut::CheckIfConnectedToBolt()
{
	HitResult hit(this);
	if (SimpleBoxCast(GetPositionV(), XMFLOAT3(0.5f, 0.5f, 0.5f), hit, false, false))
	{
		for (auto actor : hit.hitActors)
		{
			auto bolt = dynamic_cast<Bolt*>(actor);
			if (bolt)
			{
				nextPos -= GetUpVectorV() * 0.2f;
			}
		}
	}
}
