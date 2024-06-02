#include "vpch.h"
#include "Lever.h"

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

	actorToActivate = World::GetActorByNameAllowNull(actorToActivateName);
}

Properties Lever::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Actor To Activate", &actorToActivateName).useActorsAutoComplete = true;
	props.Add("Lever Active", &isLeverActive);
	return props;
}

void Lever::OnLinkRotateUp()
{
	__super::OnLinkRotateUp();

	//These checks are the make sure the Level can only move in two 90 degree increments, back and forth.
	if (!isLeverActive)
	{
		//Todo: this is no good because OnLinkRotate() is still going to be called in Player.cpp, making the 
		//audio and sprite sheet still go off on rotate.
		ResetNextRot();
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

	isLeverActive = true;
}
