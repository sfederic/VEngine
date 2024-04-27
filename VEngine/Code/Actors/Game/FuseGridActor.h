#pragma once

#include "GridActor.h"

//@Todo: there's a problem with this actor where when you initially fuse it, the new node created from the mesh
//will have the same position as the initial node. Unselecting the grid actor from link fixes it though.

//Grid actor that fuses onto another grid actor. Think like Tears of the Kingdom's fuse ability.
//FuseGridActors will probably have to be cubic to work best.
//Note that previously, the code to check grid actor next rotations was trying to work with FuseGridActors
//having multiple meshes. It didn't work well, so keep this actor functioning like how ProjectionCrystal does.
class FuseGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(FuseGridActor);

	Properties GetProps() override;
	void OnLinkMove() override;
};
