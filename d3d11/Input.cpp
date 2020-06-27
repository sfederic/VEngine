#include "Input.h"

WPARAM currentDownKey;
WPARAM currentUpKey;
bool mouseUp;

void StoreKeyDownInput(WPARAM key)
{
	currentDownKey = key;
}

void StoreKeyUpInput(WPARAM key)
{
	currentUpKey = key;
}

bool GetKeyDownState(int key)
{
	if (currentDownKey == key)
	{
		currentDownKey = 0;
		return true;
	}
	else
	{
		return false;
	}
}

bool GetKeyUpState(int key)
{
	if (currentUpKey == key)
	{
		currentUpKey = 0;
		return true;
	}
	else
	{
		return false;
	}
}
