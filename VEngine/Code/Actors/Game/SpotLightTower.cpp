#include "vpch.h"
#include "SpotLightTower.h"
#include "Components/Lights/SpotLightComponent.h"
#include "Components/MeshComponent.h"
#include "IlluminatedGridActor.h"
#include "Physics/Raycast.h"

SpotLightTower::SpotLightTower()
{
	spotLight = CreateComponent<SpotLightComponent>("SpotLight");
	rootComponent->AddChild(spotLight);
}

void SpotLightTower::Create()
{
	mesh->SetShaderItem(ShaderItems::Unlit);

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
	if (Raycast(hit, spotLight->GetWorldPositionV(), spotLight->GetForwardVectorV(), lightRange))
	{
		auto actor = hit.GetHitActorAs<IlluminatedGridActor>();
		if (actor)
		{
			actor->Illuminate();
		}
	}
}
