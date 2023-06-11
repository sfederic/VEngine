#pragma once

#include "GridActor.h"

class Polyboard;

//Two objects linked together via a PolyBoard acting as  chain/rope/etc.
class LinkedObjects : public GridActor
{
public:
	ACTOR_SYSTEM(LinkedObjects);

	LinkedObjects();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	MeshComponent* mesh2 = nullptr;
	Polyboard* link = nullptr;
};
