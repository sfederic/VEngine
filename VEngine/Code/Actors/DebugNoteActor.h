#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct NoteWidget;

//Note that is meant to be a debug-esque thing viewed from the editor,
//rendered in-world for development purposes. Got the idea from Breath of the Wild's engine from that GDC talk.
struct DebugNoteActor : Actor
{
	ACTOR_SYSTEM(DebugNoteActor);

	NoteWidget* noteWidget = nullptr;

	std::wstring noteText;

	DebugNoteActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
