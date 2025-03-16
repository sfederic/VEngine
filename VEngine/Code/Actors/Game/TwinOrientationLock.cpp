
#include "TwinOrientationLock.h"
import Core.VMath;

void TwinOrientationLock::Start()
{
	__super::Start();

	actor0 = World::Get().GetActorByNameAndLogCast<Actor>(actorName0);
	actor1 = World::Get().GetActorByNameAndLogCast<Actor>(actorName1);
}

void TwinOrientationLock::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	ActivateActorsOnCorrectOrientations();
}

Properties TwinOrientationLock::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Actor0", &actorName0).useActorsAutoComplete = true;
	props.Add("Actor1", &actorName1).useActorsAutoComplete = true;
	props.Add("Orientation0", &orientation0);
	props.Add("Orientation1", &orientation1);
	return props;
}

void TwinOrientationLock::ActivateActorsOnCorrectOrientations()
{
	auto up = GetUpVectorV();
	VMath::RoundVector(up);

	const auto orient0 = XMLoadFloat3(&orientation0);
	const auto orient1 = XMLoadFloat3(&orientation1);

	if (VMath::VecEqual(up, orient0))
	{
		if (actor0 && actor1)
		{
			actor0->Activate();
			actor1->Deactivate();
		}
	}
	else if (VMath::VecEqual(up, orient1))
	{
		if (actor1 && actor0)
		{
			actor1->Activate();
			actor0->Deactivate();
		}
	}
}
