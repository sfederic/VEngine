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
	virtual bool Hour4() { return true; }
	virtual bool Hour5() { return true; }
	virtual bool Hour6() { return true; }
	virtual bool Hour7() { return true; }
	virtual bool Hour8() { return true; }
	virtual bool Hour9() { return true; }
	virtual bool Hour10() { return true; }
	virtual bool Hour11() { return true; }
	virtual bool Hour12() { return true; }
};
