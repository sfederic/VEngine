#pragma once
#include <set>

struct Actor;
struct IActorSystem;

struct WorldEditor
{
	std::set<Actor*> pickedActors;
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
