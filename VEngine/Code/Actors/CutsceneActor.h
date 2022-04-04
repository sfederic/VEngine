#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct BoxTriggerComponent;

//Holds a reference to a cutscene file and activates it based on a trigger or calling code.
struct CutsceneActor : Actor
{
	ACTOR_SYSTEM(CutsceneActor);

	BoxTriggerComponent* boxTriggerComponent = nullptr;

	std::string cutsceneFile;

	bool playCutsceneOnTriggerOverlap = false;

	CutsceneActor();
	virtual Properties GetProps() override;

	void PlayCutscene();
};
