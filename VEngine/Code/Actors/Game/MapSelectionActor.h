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
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	//Strings to display on information widget when this actor is selected in-game. 
	std::wstring title;
	std::wstring description;

	std::string levelToLoad;
};
