#pragma once

#include "Actor.h"
#include "ActorSystem.h"
#include "Render/RenderTypes.h"

class BoxTriggerComponent;

//Actor to hold and send off data to post processing shader constant buffers.
class PostProcessVolume : public Actor
{
public:
	ACTOR_SYSTEM(PostProcessVolume);

	PostProcessVolume();
	virtual Properties GetProps() override;

	ShaderPostProcessData GetPostProcessData() { return postProcessData; }

	bool IsActiveCameraInsideVolume();

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	ShaderPostProcessData postProcessData;
};
