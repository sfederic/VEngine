#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct MeshComponent;
struct DialogueComponent;

struct NPC : Actor
{
	ACTOR_SYSTEM(NPC)

	MeshComponent* mesh = nullptr;
	DialogueComponent* dialogue = nullptr;

	NPC();
	virtual void Start() override;
	virtual Properties GetProps() override;
	void TalkTo();
};
