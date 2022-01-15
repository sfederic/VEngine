#pragma once
#include <string>

struct Player;
struct Grid;
struct Actor;

//Utility functions for in-game stuff
namespace GameUtils
{
	Player* GetPlayer();
	Grid* GetGrid();

	bool CheckIfMemoryExists(const std::string& memoryName);

	void SetActiveCameraTarget(Actor* newTarget);
	void SetActiveCameraTargetAndZoomIn(Actor* newTarget);
	void SetActiveCameraTargetAndZoomOut(Actor* newTarget);

	void PlayAudioOneShot(const std::string audioFilename);
	void SaveGameWorldState();
	void LoadWorld(std::string worldName);

	inline static const std::string gameInstanceSaveFile = "GameSaves/game_instance.sav";

	void SaveGameInstanceData();
	void LoadGameInstanceData();

	//Used with timers to be able to call LoadWorldAndMoveToEntranceTrigger() through Timer.
	extern std::string levelToMoveTo;

	//Move to matching entrance trigger with same name as exited one in loaded world.
	void LoadWorldAndMoveToEntranceTrigger();

	void TriggerGameOver();
};
