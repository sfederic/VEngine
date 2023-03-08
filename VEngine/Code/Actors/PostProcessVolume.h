#pragma once

#include "Actor.h"
#include "ActorSystem.h"
#include "Render/RenderTypes.h"

class BoxTriggerComponent;

class PostProcessVolume : public Actor
{
public:
	ACTOR_SYSTEM(PostProcessVolume);

	PostProcessVolume();
	Properties GetProps() override;

	ShaderPostProcessData GetPostProcessData() { return postProcessData; }

	bool IsActiveCameraInsideVolume();

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	ShaderPostProcessData postProcessData;
};
