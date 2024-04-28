#include "vpch.h"
#include "BombFlower.h"
#include "Components/MeshComponent.h"

void BombFlower::Create()
{
	__super::Create();

	mesh->SetMeshFilename("sphere.vmesh");
	mesh->SetWorldScale(0.5f);
}

void BombFlower::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (setToExplode)
	{
		explodeTimer += deltaTime;

		constexpr float explodeTimerMax = 3.0f;
		mesh->SetAmbientColour(XMFLOAT3(explodeTimer / explodeTimerMax, 0.f, 0.f));

		if (explodeTimer > explodeTimerMax)
		{
			this->Burn();
		}
	}
}

Properties BombFlower::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void BombFlower::OnLinkActivate()
{
	__super::OnLinkActivate();

	setToExplode = true;
}
