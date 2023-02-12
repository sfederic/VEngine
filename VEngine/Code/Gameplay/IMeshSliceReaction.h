#pragma once

//Throw onto actors that need functions called when slicable meshes and actors that are linked to them are cut.
class IMeshSliceReaction
{
public:
	virtual void OnSlicedLinkedActor() = 0;
};
