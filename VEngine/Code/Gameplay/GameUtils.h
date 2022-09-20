#pragma once

#include <string>
#include <DirectXMath.h>

using namespace DirectX;

class Player;
class Actor;
struct SpriteSheet;

//Utility functions for in-game stuff.
namespace GameUtils
{
	Player* GetPlayer();
	void SetPlayerCombatOn();
	void SetPlayerCombatOff();

	void SetActiveCameraTarget(Actor* newTarget);
	void CameraShake(float shake);

	SpriteSheet* SpawnSpriteSheet(std::string textureFilename, XMFLOAT3 position, bool loop, int numRows, int numColumns);

	void PlayAudioOneShot(const std::string audioFilename);

	void SaveGameWorldState();
	void LoadWorld(std::string worldName);

	inline static const std::string gameInstanceSaveFile = "GameSaves/game_instance.sav";

	void SaveGameInstanceData();
	void LoadGameInstanceData();

	//Used with timers to be able to call LoadWorldAndMoveToEntranceTrigger() through Timer.
	extern std::string levelToMoveTo;
};
