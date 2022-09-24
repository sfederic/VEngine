#pragma once

struct Mission;

namespace MissionSystem
{
	void Init();
	Mission* FindMission(std::wstring name);
};
