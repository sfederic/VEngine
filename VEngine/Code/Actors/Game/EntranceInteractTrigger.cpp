#include "vpch.h"
#include "EntranceInteractTrigger.h"
#include "Core/Input.h"

void EntranceInteractTrigger::Tick(float deltaTime)
{
	if (Input::GetKeyUp(Keys::Down))
	{
		__super::Tick(deltaTime);
	}
}
