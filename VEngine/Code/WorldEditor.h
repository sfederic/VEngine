#pragma once

struct Actor;

struct WorldEditor
{
	Actor* pickedActor;

	void Tick();
	void HandleActorPicking();
	void DuplicateActor();
};

extern WorldEditor worldEditor;
