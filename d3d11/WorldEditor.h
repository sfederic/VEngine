#pragma once

#include "Raycast.h"

class WorldEditor
{
public:
	void Tick();

	Ray screenPickRay;
	int actorIndex;
	int actorSystemIndex;
};

extern WorldEditor gWorldEditor;
