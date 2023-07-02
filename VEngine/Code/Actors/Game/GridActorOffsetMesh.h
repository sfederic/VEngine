#pragma once

#include "GridActor.h"

//Grid actor where the main mesh is child of a empty root to enable offset rotations during gameplay.
class GridActorOffsetMesh : public GridActor
{
public:
	ACTOR_SYSTEM(GridActorOffsetMesh);

	GridActorOffsetMesh();

	bool CheckNextRotationBoundsIntersect() override;
};
