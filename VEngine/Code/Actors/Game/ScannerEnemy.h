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

private:
	void ScanForPlayer();
	void RotateWhileScanning();

	Polyboard* scanLaser = nullptr;

	enum class ScanMode
	{
		Scanning,
		LockedOnToPlayer
	};

	ScanMode scanMode = ScanMode::Scanning;

	float scanTimer = 0.f;
};
