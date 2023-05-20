#pragma once

#include "GridActor.h"

class SpriteSheet;

class SteamValve : public GridActor
{
public:
	ACTOR_SYSTEM(SteamValve);

	void Create() override;
	void OnLinkRotate() override;

private:
	SpriteSheet* steamSprite = nullptr;
	bool isValveOn = false;
};
