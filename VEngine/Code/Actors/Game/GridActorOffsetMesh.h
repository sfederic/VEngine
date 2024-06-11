#pragma once

#include "GridActor.h"

//Grid actor where the main mesh is child of a empty root to enable offset rotations during gameplay.

//Note: GridActorOffsetMesh doesn't work with this GridActor::CheckNextRotationBoundsIntersect() that well.
//Need to take the concession that this will be used for actors that can be moved and rotated without a 
//care for bounds checking against other actors.
class GridActorOffsetMesh : public GridActor
{
public:
	ACTOR_SYSTEM(GridActorOffsetMesh);

	GridActorOffsetMesh();
};
