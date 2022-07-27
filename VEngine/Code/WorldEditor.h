#pragma once
#include <set>
#include <string>

class Actor;
class IActorSystem;
struct Transform;
struct SpatialComponent;

enum class PickMode
{
	Actor,
	Component
};

struct WorldEditor
{
	std::set<Actor*> pickedActors;
	Actor* pickedActor = nullptr;

	SpatialComponent* pickedComponent = nullptr;

	IActorSystem* spawnSystem = nullptr;
	std::string actorTemplateFilename;

	PickMode pickMode = PickMode::Actor;

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
