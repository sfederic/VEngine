#pragma once

#include <set>
#include <string>
#include <DirectXMath.h>

class Actor;
class IActorSystem;
struct SpatialComponent;
struct Transform;

//@Todo: actor delete undo

namespace WorldEditor
{
	enum class PickMode
	{
		Actor,
		Component
	};

	extern bool texturePlacement;
	extern bool meshPlacement;
	extern bool materialPlacement;
	extern bool vertexPaintActive;
	extern bool actorReplaceModeActive;
	extern DirectX::XMFLOAT4 vertexPaintColour;
	
	void Tick();

	void DeselectPickedActor();
	void DeselectAll();

	void SetPickedActor(Actor* actor);
	Actor* GetPickedActor();

	void AddPickedActor(Actor* actor);
	std::set<Actor*>& GetPickedActors();
	void ClearPickedActors();

	void SetPickedComponent(SpatialComponent* spatialComponent);
	SpatialComponent* GetPickedComponent();

	void SetSpawnSystem(IActorSystem* newSpawnSystem);
	IActorSystem* GetSpawnSystem();

	void SetPickMode(PickMode newPickMode);
	PickMode GetPickMode();

	std::string GetActorTempateFilename();
	void SetActorTemplateFilename(std::string netActorTempateFilename);

	Actor* SpawnActorFromTemplateFile(std::string templateFilename, Transform& transform);
};
