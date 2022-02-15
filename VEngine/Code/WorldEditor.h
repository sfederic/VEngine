#pragma once
#include <set>
#include <string>

struct Actor;
struct IActorSystem;
struct Transform;

struct WorldEditor
{
	std::set<Actor*> pickedActors;
	Actor* pickedActor = nullptr;
	IActorSystem* spawnSystem = nullptr;
	std::string actorTemplateFilename;

	void Tick();
	void DeselectPickedActor();
	void SetPickedActor(Actor* actor);

private:
	void HandleActorPicking();
	void DuplicateActor();
	void SaveWorld();
	void DeleteActor();
	void SpawnActorOnClick();
	void SpawnActor(Transform& transform);
};

extern WorldEditor worldEditor;
