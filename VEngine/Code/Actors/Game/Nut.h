#pragma once

#include "GridActor.h"

//@Todo: code here is rough but working visually. Needs checks against next position against Bolt.

//Metallic nut, not the squirrel nut.
class Nut : public GridActor
{
public:
	ACTOR_SYSTEM(Nut);

	void Create() override;

	void OnLinkRotateLeft() override;
	void OnLinkRotateRight() override;

private: 
	void CheckIfConnectedToBolt(const XMVECTOR moveDirection);
};
