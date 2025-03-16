#pragma once

#include "Actors/Game/GridActor.h"
import <vector>;

//Fruitbasket is an actor holding fruits that are spawned as physics when flipped upside down.
//It's a "fruit basket", but you can use it for anything.
class FruitBasket : public GridActor
{
public:
	ACTOR_SYSTEM(FruitBasket);

	FruitBasket();
	void Tick(float deltaTime) override;
	void End() override;
	Properties GetProps() override;

private:
	std::vector<MeshComponent*> physicsMeshes;

	class BoxTriggerComponent* meshSpawnTrigger = nullptr;

	bool hasBeenEmptied = false;
};
