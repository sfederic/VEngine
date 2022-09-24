#include "vpch.h"
#include "MissionManager.h"
#include <map>
#include "Mission.h"

std::map<std::string, Mission> missionMap;

void AddMission(Mission mission)
{
	missionMap.emplace(mission);
}

void MissionManager::Init()
{
	AddMission(Mission(L"Test Mission", L"Take photos and destroy 3 enemies also do this and that." ));
}
