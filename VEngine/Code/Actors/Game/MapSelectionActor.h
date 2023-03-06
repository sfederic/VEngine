#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//An actor the MapScreenSelector can select on the map, holding information for the level to enter via map.
//Triggers when the MapScreenSelector is inside this actor's boxtrigger, that way you can place meshes around it.
class MapSelectionActor : public Actor
{
public:
	ACTOR_SYSTEM(MapSelectionActor);

	MapSelectionActor();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	auto GetLevelToLoad() { return levelToLoad; }

	//Where the player will spawn back into relative to this actor on when coming back to the map screen.
	XMVECTOR GetPlayerReturnSpawnPoint();

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	//Strings to display on information widget when this actor is selected in-game. 
	std::wstring title;
	std::wstring description;

	std::string levelToLoad;

	std::string gloabalPropToCheckOnActivate;
};
