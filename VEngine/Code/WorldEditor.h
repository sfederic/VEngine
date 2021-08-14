#pragma once

struct Actor;

struct WorldEditor
{
	Actor* pickedActor;

	void Tick();
	void HandleActorPicking();
};

extern WorldEditor worldEditor;
