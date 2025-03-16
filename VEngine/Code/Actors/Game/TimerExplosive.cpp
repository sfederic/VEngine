
#include "TimerExplosive.h"
import Components.MeshComponent;

void TimerExplosive::Create()
{
	__super::Create();

	_mesh->SetMeshFilename("sphere.vmesh");
	_mesh->SetWorldScale(0.5f);
}

void TimerExplosive::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (setToExplode)
	{
		explodeTimer += deltaTime;

		constexpr float explodeTimerMax = 3.0f;
		_mesh->SetAmbientColour(XMFLOAT3(explodeTimer / explodeTimerMax, 0.f, 0.f));

		if (explodeTimer > explodeTimerMax)
		{
			this->Burn();
		}
	}
}

Properties TimerExplosive::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void TimerExplosive::OnLinkActivate()
{
	__super::OnLinkActivate();

	setToExplode = true;
}
