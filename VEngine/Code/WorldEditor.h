#pragma once
#include <vector>

struct Actor;
struct IActorSystem;

struct WorldEditor
{
	std::vector<Actor*> pickedActors;
	Actor* pickedActor = nullptr;
	IActorSystem* spawnSystem = nullptr;

	void Tick();
	void HandleActorPicking();
	void DuplicateActor();
	void SaveWorld();
	void DeleteActor();
	void SpawnActorOnClick();
	void DeselectPickedActor();
	void SetPickedActor(Actor* actor);
};

extern WorldEditor worldEditor;
