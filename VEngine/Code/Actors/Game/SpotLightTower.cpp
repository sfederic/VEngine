
#include "SpotLightTower.h"
#include "Components/Lights/SpotLightComponent.h"
import Components.MeshComponent;
#include "IlluminatedGridActor.h"
import Physics.Raycast;

SpotLightTower::SpotLightTower()
{
	spotLight = CreateComponent<SpotLightComponent>("SpotLight");
	AddChildToRoot(spotLight);
}

void SpotLightTower::Create()
{
	__super::Create();

	_mesh->SetShaderItem("Unlit");

	spotLight->SetLocalPosition(0.f, 1.f, 0.f);
}

void SpotLightTower::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	IlluminateGridActors();
}

void SpotLightTower::IlluminateGridActors()
{
	HitResult hit(this);
	if (Physics::Raycast(hit, spotLight->GetWorldPositionV(), spotLight->GetForwardVectorV(), lightRange))
	{
		auto actor = hit.GetHitActorAs<IlluminatedGridActor>();
		if (actor)
		{
			actor->Illuminate();
		}
	}
}
