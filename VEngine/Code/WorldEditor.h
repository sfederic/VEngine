#pragma once

struct Actor;
struct IActorSystem;

struct WorldEditor
{
	Actor* pickedActor = nullptr;
	IActorSystem* spawnSystem = nullptr;

	bool spawnerActive = false;

	void Tick();
	void HandleActorPicking();
	void DuplicateActor();
	void SaveWorld();
	void DeleteActor();
	void SpawnActorOnClick();
};

extern WorldEditor worldEditor;
