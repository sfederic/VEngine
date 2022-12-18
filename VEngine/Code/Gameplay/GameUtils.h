#pragma once

#include <string>
#include <DirectXMath.h>

using namespace DirectX;

class Actor;
struct SpriteSheet;
class CameraComponent;

//Utility functions for in-game stuff
namespace GameUtils
{
	bool CheckIfMemoryExists(const std::string memoryName);

	void SetActiveCameraTarget(Actor* newTarget);
	void SetActiveCameraTargetAndZoomIn(Actor* newTarget);
	void SetActiveCameraTargetAndZoomOut(Actor* newTarget);

	//Mainly for timer based calls.
	void SetCameraBackToPlayer();

	void CameraShake(float shake);

	SpriteSheet* SpawnSpriteSheet(std::string textureFilename, XMVECTOR position, bool loop, int numRows, int numColumns);

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

	void DisablePlayer();

	void SetActiveCamera(CameraComponent* camera);
};
