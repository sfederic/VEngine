#include "vpch.h"
#include "TimeOfDayWheel.h"
#include "Core/VMath.h"
#include "Gameplay/GameInstance.h"

void TimeOfDayWheel::OnRotationEnd()
{
	__super::OnRotationEnd();

	auto up = GetUpVectorV();
	VMath::RoundVector(up);

	//Change time of day global prop based on rotation orientation
	if (VMath::VecEqual(up, VMath::GlobalUpVector()))
	{
		GameInstance::SetGlobalProp("TownTimeOfDay", "Morning");
		Log("Town time of day set to Morning");
	}
	else if (VMath::VecEqual(up, VMath::GlobalRightVector()))
	{
		GameInstance::SetGlobalProp("TownTimeOfDay", "Day");
		Log("Town time of day set to Day");
	}
	else if (VMath::VecEqual(up, -VMath::GlobalUpVector()))
	{
		GameInstance::SetGlobalProp("TownTimeOfDay", "Sunset");
		Log("Town time of day set to Sunset");
	}
	else if (VMath::VecEqual(up, -VMath::GlobalRightVector()))
	{
		GameInstance::SetGlobalProp("TownTimeOfDay", "Night");
		Log("Town time of day set to Night");
	}
}

Properties TimeOfDayWheel::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
