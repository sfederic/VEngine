#pragma once

#include "ScannerEnemy.h"

class ScoutingScannerEnemy : public ScannerEnemy
{
public:
	ACTOR_SYSTEM(ScoutingScannerEnemy);

	void Tick(float deltaTime);
	Properties GetProps() override;

private:
	void MoveToNextNode(float deltaTime);

	float moveTimer = 0.f;
};
