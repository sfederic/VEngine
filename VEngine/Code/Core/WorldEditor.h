#pragma once

#include <set>
#include <string>
#include <DirectXMath.h>

class Actor;
class IActorSystem;
struct SpatialComponent;
struct Transform;

//@Todo: actor delete undo

struct UVPaintData
{
	std::string texture;
	float x = 0.f;
	float y = 0.f;
	float w = 0.f;
	float h = 0.f;
};

namespace WorldEditor
{
	enum class PickMode
	{
		Actor,
		Component
	};

	extern UVPaintData uvPaintData;

	extern bool texturePlacement;
	extern bool meshPlacement;
	extern bool materialPlacement;
	extern bool vertexPaintActive;
	extern bool uvPaintActive;
	extern bool actorReplaceModeActive;
	extern bool parentSetActive;
	extern bool moveActorViaKeyboardInput;
	extern bool entranceTriggerWorldLoadMode;

	extern DirectX::XMFLOAT4 vertexPaintColour;

	//Determines whether vertex colouring paints to all three verts of a hit face or the closest vert on click.
	extern bool vertexPaintFaceFillMode;

	//In vertex paint mode, setting this actor in the debug menu will denote that only raycasts
	//will hit and paint this actor, making it paintable when its occluded by other meshes.
	extern Actor* vertexPaintLockActor;

	void Tick();
	void Reset();

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
