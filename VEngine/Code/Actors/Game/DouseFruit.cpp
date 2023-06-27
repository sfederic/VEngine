#include "vpch.h"
#include "DouseFruit.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameUtils.h"

void DouseFruit::Create()
{
	mesh->SetMeshFilename("small_ico_sphere.vmesh");
	mesh->SetTexture("watermelon.jpg");
	mesh->SetLocalScale(0.33f);
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

void DouseFruit::OnLinkDeactivate()
{
	//Drop fruit (Don't want it hanging around as a grid actor you can step on)
	if (!hasBeenDoused)
	{
		GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", GetPositionV(), false, 5, 5);
		Destroy();
	}
}
