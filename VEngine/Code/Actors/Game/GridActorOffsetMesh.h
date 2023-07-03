#pragma once

#include "GridActor.h"

//@Todo: GridActorOffsetMesh doesn't work with this GridActor::CheckNextRotationBoundsIntersect().
//Will need to change the func around to work with non-root meshes.

//Grid actor where the main mesh is child of a empty root to enable offset rotations during gameplay.
class GridActorOffsetMesh : public GridActor
{
public:
	ACTOR_SYSTEM(GridActorOffsetMesh);

	GridActorOffsetMesh();
};
