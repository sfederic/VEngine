#pragma once

//Quest data that quests inherit from and define their events via the HourN() calls.
struct Quest
{
	bool isActive = true;

	Quest(const char* questName);
	bool ActivateOnHour();

private:
	virtual bool Hour0() { return true; }
	virtual bool Hour1() { return true; }
	virtual bool Hour2() { return true; }
	virtual bool Hour3() { return true; }
};
