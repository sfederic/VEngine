#pragma once

#include "GridActor.h"

//Grid actor that breaks when moved, shattering the window.
//(Note that it's called WindowGridActor because everything in every API ever is called "Window".)
class WindowGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(WindowGridActor);

	void Create() override;

	void OnLinkMove() override;
	void OnLinkRotate() override;
};
