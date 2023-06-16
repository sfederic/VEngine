#include "vpch.h"
#include "DouseFruit.h"
#include "Components/MeshComponent.h"

void DouseFruit::Create()
{
	canBeMovedInLink = false;
	canBeRotatedInLink = false;

	mesh->SetMeshFilename("small_ico_sphere.vmesh");
	mesh->SetTexture("watermelon.jpg");
	mesh->SetLocalScale(0.25f);
}

void DouseFruit::Douse()
{
	if (hasBeenDoused)
	{
		return;
	}

	hasBeenDoused = true;

	canBeMovedInLink = true;
	canBeRotatedInLink = true;

	mesh->SetMeshFilename("cube.vmesh");
	mesh->ReCreate();
	mesh->SetLocalScale(1.f);
}
