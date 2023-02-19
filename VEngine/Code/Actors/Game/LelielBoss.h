#pragma once

#include "Enemy.h"

class MeshComponent;

//Idea taken from the eva angel in episode 16 and Yoshi's Story, that invisible boss (Inviso).
//Boss is invisible and casts a shadow on surrounding objects, forcing player to find its actual spot.
class LelielBoss : public Enemy
{
public:
	ACTOR_SYSTEM(LelielBoss);

	LelielBoss();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;

private:
	MeshComponent* mesh = nullptr;
	XMVECTOR nextPosToMoveTo= XMVectorSet(0.f, 0.f, 0.f, 1.f);
	float moveSpeed = 1.f;
};
