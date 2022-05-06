#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct ShaderPostProcessData;

//Actor to hold and send off data to post processing shader constant buffers.
//Should only have one of these per level. 
//Most engines use a volume, current game is small enough to use a global level-wide instance.
struct PostProcessInstance : Actor
{
	ACTOR_SYSTEM(PostProcessInstance);

	float gamma = 1.0f;
	float exposure = 1.0f;

	PostProcessInstance();
	virtual Properties GetProps() override;
	void SetShaderPostProcessData(ShaderPostProcessData& data);
};
