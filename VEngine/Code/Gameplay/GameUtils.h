#pragma once
#include <string>

struct Player;
struct Grid;

//Utility functions for in-game stuff
namespace GameUtils
{
	Player* GetPlayer();
	Grid* GetGrid();
	void PlayAudio(const std::string audioFilename);
	void SaveGameWorldState();
	void LoadWorld(std::string worldName);

	//Move to matching entrance trigger with same name as exited one in loaded world.
	void LoadWorldAndMoveToEntranceTrigger(std::string worldName);
};
