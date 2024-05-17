#include "vpch.h"
#include "DouseFruit.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameUtils.h"

void DouseFruit::Create()
{
	__super::Create();

	_mesh->SetMeshFilename("small_ico_sphere.vmesh");
	_mesh->SetTexture("watermelon.jpg");
	_mesh->SetLocalScale(0.33f);
}

void DouseFruit::Douse()
{
	__super::Douse();

	if (hasBeenDoused)
	{
		return;
	}

	hasBeenDoused = true;

	canBeMovedInLink = true;
	canBeRotatedInLink = true;

	_mesh->SetMeshFilename("cube.vmesh");
	_mesh->ReCreate();
	_mesh->SetLocalScale(1.f);
}

void DouseFruit::OnLinkDeactivate()
{
	__super::OnLinkDeactivate();

	//Drop fruit (Don't want it hanging around as a grid actor you can step on)
	if (!hasBeenDoused)
	{
		GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", GetPositionV(), false, 5, 5);
		Remove();
	}
}
