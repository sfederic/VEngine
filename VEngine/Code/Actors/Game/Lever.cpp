#include "vpch.h"
#include "Lever.h"
#include "Gameplay/GameUtils.h"

void Lever::Create()
{
	__super::Create();

	SetMeshFilename("lever.vmesh");

	canBeMovedInLink = false;
	canBeRotatedRollZAxis = false;
	canBeRotatedYawYAxis = false;
}

void Lever::Start()
{
	__super::Start();

	actorToActivate = World::Get().GetActorByNameAllowNull(actorToActivateName);
}

Properties Lever::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Actor To Activate", &actorToActivateName).useActorsAutoComplete = true;
	props.Add("Lever Active", &isLeverActive);
	return props;
}

//Careful with this function. It's purposefully not calling super::OnLinkRotate() to avoid conflicting
//events in OnLinkRotateUp and Down.
void Lever::OnLinkRotate()
{
	isRotating = true;
}

void Lever::OnLinkRotateUp()
{
	__super::OnLinkRotateUp();

	//These checks are the make sure the Level can only move in two 90 degree increments, back and forth.
	if (!isLeverActive)
	{
		ResetNextRot();
	}
	else
	{
		GameUtils::PlayAudioOneShot(linkRotateAudio);
	}

	isLeverActive = false;
}

void Lever::OnLinkRotateDown()
{
	__super::OnLinkRotateDown();

	if (isLeverActive)
	{
		ResetNextRot();
	}
	else
	{
		GameUtils::PlayAudioOneShot(linkRotateAudio);
	}

	isLeverActive = true;
}
