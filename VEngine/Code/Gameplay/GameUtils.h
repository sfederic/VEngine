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
	void SetActiveCameraTarget(Actor* newTarget);
	void PlayAudio(const std::string audioFilename);
	void SaveGameWorldState();
	void LoadWorld(std::string worldName);

	inline static const std::string gameInstanceSaveFile = "GameSaves/game_instance.sav";

	void SaveGameInstanceData();
	void LoadGameInstanceData();

	//Move to matching entrance trigger with same name as exited one in loaded world.
	void LoadWorldAndMoveToEntranceTrigger(std::string worldName);
};
