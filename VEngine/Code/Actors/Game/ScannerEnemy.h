#pragma once

#include "Unit.h"

class ScanningEnemyAlertWidget;
class Polyboard;

//Enemy that scans around, prompting the player to use GridActors as cover from its sight.
//Stole the idea from Ocarina's Beamos enemies, those eye statue enemies that shoot lasers.
class ScannerEnemy : public Unit
{
public:
	ACTOR_SYSTEM(ScannerEnemy);

	ScannerEnemy();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

protected:
	void ScanForPlayer(float deltaTime);

	enum class ScanMode
	{
		Scanning,
		LockedOnToPlayer
	};

	Polyboard* scanLaser = nullptr;
	ScanningEnemyAlertWidget* alertWidget = nullptr;
	ScanMode scanMode = ScanMode::Scanning;

	//Set this to 0 if you want stationary ScannerEnemies
	float spinSpeed = 20.f;
};
