#pragma once

#include "GridActor.h"

class PointLightComponent;

class OnOffPointLight : public GridActor
{
public:
	ACTOR_SYSTEM(OnOffPointLight);

	OnOffPointLight();
	void Create() override;
	void Start() override;
	Properties GetProps() override;

	void Activate() override;
	void Deactivate() override;

private:
	PointLightComponent* pointLight = nullptr;
	bool isOn = false;
};
