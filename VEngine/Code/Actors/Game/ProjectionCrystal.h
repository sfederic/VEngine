#pragma once

#include "GridActor.h"

class MeshComponent;
class SpotLightComponent;

//Projects a 'scene' into the world.
class ProjectionCrystal : public GridActor
{
public:
	ACTOR_SYSTEM(ProjectionCrystal);

	ProjectionCrystal();
	void Start() override;
	void Interact() override;
	void OnLinkActivate() override;
	void OnLinkDeactivate() override;

private:
	MeshComponent* projectionMesh = nullptr;
	SpotLightComponent* projectionSpotLight = nullptr;

	bool isActive = false;
};
