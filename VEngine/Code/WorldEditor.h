#pragma once

struct Actor;
struct IActorSystem;

struct WorldEditor
{
	Actor* pickedActor = nullptr;
	IActorSystem* spawnSystem = nullptr;

	void Tick();
	void HandleActorPicking();
	void DuplicateActor();
	void SaveWorld();
	void DeleteActor();
	void SpawnActorOnClick();
	void DeselectPickedActor();
};

extern WorldEditor worldEditor;
