#pragma once

//Quest data that quests inherit from and define their events via the HourN() calls.
struct Quest
{
	virtual bool Hour0() {}
	virtual bool Hour1() {}
	virtual bool Hour2() {}
	virtual bool Hour3() {}
};
