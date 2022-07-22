#pragma once
#include <set>
#include <string>

struct Actor;
class IActorSystem;
struct Transform;
struct SpatialComponent;

struct WorldEditor
{
	std::set<Actor*> pickedActors;
	Actor* pickedActor = nullptr;

	SpatialComponent* pickedComponent = nullptr;

	IActorSystem* spawnSystem = nullptr;
	std::string actorTemplateFilename;

	void Tick();
	void DeselectPickedActor();
	void SetPickedActor(Actor* actor);
	void SetPickedComponent(SpatialComponent* spatialComponent);

private:
	void HandleActorPicking();
	void DuplicateActor();
	void SaveWorld();
	void DeleteActor();
	void SpawnActorOnClick();
	void SpawnActor(Transform& transform);
};

extern WorldEditor worldEditor;
