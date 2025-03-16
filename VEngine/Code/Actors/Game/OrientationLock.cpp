
#include "OrientationLock.h"
import Core.VMath;
#include "Core/Log.h"
#include "Player.h"

void OrientationLock::Start()
{
	__super::Start();

	linkedGridActor = World::Get().GetActorByNameAndLogCast<GridActor>(gridActorToActivateOnCorrectOrientation);
}

void OrientationLock::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (linkedGridActor && !hasBeenFittedToOrientation)
	{
		if (IsOrientationCorrect())
		{
			linkedGridActor->Activate();
		}
		else
		{
			linkedGridActor->Deactivate();
		}
	}
}

Properties OrientationLock::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Dir Forward", &forward);
	props.Add("Dir Up", &up);
	props.Add("Dir Right", &right);
	props.Add("Linked GridActor", &gridActorToActivateOnCorrectOrientation);
	return props;
}

bool OrientationLock::IsOrientationCorrect()
{
	auto forwardVector = GetForwardVectorV();
	auto upVector = GetUpVectorV();
	auto rightVector = GetRightVectorV();

	VMath::RoundVector(forwardVector);
	VMath::RoundVector(upVector);
	VMath::RoundVector(rightVector);

	auto dirForward = XMLoadFloat3(&forward);
	auto dirUp = XMLoadFloat3(&up);
	auto dirRight = XMLoadFloat3(&right);

	int neededOrientations = 3;

	if (VMath::VecEqual(XMVectorZero(), dirForward)) { neededOrientations--; }
	if (VMath::VecEqual(XMVectorZero(), dirRight)) { neededOrientations--; }
	if (VMath::VecEqual(XMVectorZero(), dirUp)) { neededOrientations--; }

	bool forwardEqual = VMath::VecEqual(forwardVector, dirForward);
	bool rightEqual = VMath::VecEqual(rightVector, dirRight);
	bool upEqual = VMath::VecEqual(upVector, dirUp);

	int finalMatchingOrientations = forwardEqual + rightEqual + upEqual;
	return finalMatchingOrientations == neededOrientations;
}

void OrientationLock::FitIntoOrientationTrigger()
{
	hasBeenFittedToOrientation = true;

	canBeMovedInLink = false;
	canBeRotatedInLink = false;

	Player::system.GetOnlyActor()->ResetLinkedGridActorIfThis(this);
}
