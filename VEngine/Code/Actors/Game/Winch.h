#pragma once

#include "GridActor.h"

//Winch just moves a grid actor back and forth.
class Winch : public GridActor
{
public:
	ACTOR_SYSTEM(Winch);

	void Create() override;
	void Start() override;
	Properties GetProps() override;

	void OnLinkRotateLeft() override;
	void OnLinkRotateRight() override;

private:
	XMFLOAT3 moveDirection;
	std::string linkedActorName;
	GridActor* linkedActor = nullptr;
	float moveIncrement = 1.0f;
};
