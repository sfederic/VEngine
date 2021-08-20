#pragma once

struct Actor;

struct WorldEditor
{
	Actor* pickedActor;

	void Tick();
	void HandleActorPicking();
	void DuplicateActor();
	void SaveWorld();
};

extern WorldEditor worldEditor;
