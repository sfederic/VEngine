#pragma once

#include "GridActor.h"
#include <DirectXMath.h>

class Lift;

//Used to move a Lift grid actor.
class LiftCrank : public GridActor
{
public:
	ACTOR_SYSTEM(LiftCrank);

	void Create() override;
	void Start() override;
	Properties GetProps() override;

	void OnLinkRotateLeft() override;
	void OnLinkRotateRight() override;

private:
	void MoveLinkedLiftAndContainedActors(const DirectX::XMVECTOR direction);

	std::string linkedLiftName;
	Lift* linkedLift = nullptr;

	DirectX::XMFLOAT3 moveDirection = DirectX::XMFLOAT3(0.f, 1.f, 0.f);
};
