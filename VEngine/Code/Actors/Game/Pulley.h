#pragma once

#include "GridActor.h"

//Pulley is a rotatable on the spot, used to pull in something attached to it, 
//like how a ski chair lift would work. Works both ways (pull and slack).
class Pulley : public GridActor
{
public:
	ACTOR_SYSTEM(Pulley);

	void Start() override;
	void Create() override;
	Properties GetProps() override;

	void OnLinkRotateLeft() override;
	void OnLinkRotateRight() override;

private:
	void ReelActorIn();
	void ReelActorOut();

	std::string pullActorName;
	GridActor* pullActor = nullptr;
	XMFLOAT3 pullDirection;
	float pullIncrement = 1.f;

	//The current 'index' the pulley is on per each direction.
	int pullIndex = 0;
	int pullMin = 0;
	int pullMax = 3;
};
