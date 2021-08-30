#pragma once

struct Actor;

struct WorldEditor
{
	Actor* pickedActor;

	void Tick();
	void HandleActorPicking();
	void DuplicateActor();
	void SaveWorld();
	void DeleteActor();
	void SpawnActorOnClick();
};

extern WorldEditor worldEditor;
