#pragma once

#include "Unit.h"

class Polyboard;

//Enemy that scans around, prompting the player to use GridActors as cover from its sight.
//Stole the idea from Ocarina's Beamos enemies, those eye statue enemies that shoot lasers.
class ScannerEnemy : public Unit
{
public:
	ACTOR_SYSTEM(ScannerEnemy);

	ScannerEnemy();
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

protected:
	void ScanForPlayer(float deltaTime);

	Polyboard* scanLaser = nullptr;

	enum class ScanMode
	{
		Scanning,
		LockedOnToPlayer
	};

	ScanMode scanMode = ScanMode::Scanning;
};
