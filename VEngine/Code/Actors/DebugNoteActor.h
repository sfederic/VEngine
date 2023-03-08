#pragma once

#include "Actor.h"
#include "ActorSystem.h"

struct DebugNoteWidget;

//Note that is meant to be a debug-esque thing viewed during gameplay, rendered in-world for development purposes.
//Got the idea from Breath of the Wild's engine from that GDC talk.
class DebugNoteActor : public Actor
{
public:
	ACTOR_SYSTEM(DebugNoteActor);

	DebugNoteActor();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	DebugNoteWidget* noteWidget = nullptr;
	std::wstring noteText;
};
