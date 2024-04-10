#pragma once

#include "GridActor.h"

//A grid actor that changes shape every time it is moved while linked.
class TransformativeGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(TransformativeGridActor);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void OnLinkMove() override;

private:
	void ChangeMeshShape();

	std::string originalMeshFilename = "cube.vmesh";
	std::string changeMeshFilename = "barrel.vmesh";
	bool changeMeshActive = false;

	bool scalingDown = false;
	bool scalingUp = false;
};
