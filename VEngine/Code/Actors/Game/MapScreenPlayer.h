#pragma once

#include "Player.h"

//Limited player class for map screen that basically only has movement and trigger logic.
class MapScreenPlayer : public Player
{
public:
	ACTOR_SYSTEM(MapScreenPlayer);

	MapScreenPlayer() : Player() {}
	void Create() override { __super::Create(); }
	void Start() override;
	void Tick(float deltaTime) override;
};
