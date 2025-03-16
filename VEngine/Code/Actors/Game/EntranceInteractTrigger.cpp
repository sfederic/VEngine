
#include "EntranceInteractTrigger.h"
#include "Core/Input.h"

void EntranceInteractTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (Input::GetKeyUp("Interact"))
	{
		__super::Tick(deltaTime);
	}
}
