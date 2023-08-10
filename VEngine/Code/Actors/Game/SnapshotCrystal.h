#pragma once

#include "ProjectionCrystal.h"

//Takes a snapshot of a gridactor it's facing to work as a mesh it projects out
class SnapshotCrystal : public ProjectionCrystal
{
public:
	ACTOR_SYSTEM(SnapshotCrystal);

	void Create() override;
	void Interact() override;

private:
	void SetProjectionMeshFromFacingGridActor();
};
