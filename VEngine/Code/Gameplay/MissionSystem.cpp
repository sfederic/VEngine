#include "vpch.h"
#include "MissionSystem.h"
#include <map>
#include "Mission.h"

std::map<std::wstring, Mission> missionMap;

void AddMission(Mission mission)
{
	missionMap.emplace(mission.name, mission);
}

void MissionSystem::Init()
{
	AddMission(Mission(L"Test Mission", L"Take photos and destroy 3 enemies also do this and that." ));
}

Mission* MissionSystem::FindMission(std::wstring name)
{
	auto missionIt = missionMap.find(name);
	assert(missionIt != missionMap.end());
	return &missionIt->second;
}
