#pragma once

#include "GridActor.h"

//Moves up and down when you rotate it on the Y-axis.
class CorkscrewLift : public GridActor
{
public:
	ACTOR_SYSTEM(CorkscrewLift);

	void Create() override;

	Properties GetProps() override
	{
		auto props = __super::GetProps();
		props.title = GetTypeName();
		return props;
	}

	void OnLinkRotateRight() override;
	void OnLinkRotateLeft() override;
};
