#pragma once

#include <string>
#include <DirectXMath.h>

using namespace DirectX;

class Actor;
class SpriteSheet;
class BoxTriggerComponent;
class CameraComponent;
class Polyboard;
class ParticleEmitter;

//Utility functions for in-game stuff
namespace GameUtils
{
	SpriteSheet* SpawnSpriteSheet(std::string textureFilename, XMVECTOR spawnPosition, bool loop, int numRows, int numColumns);
	Polyboard* SpawnPolyboard(const std::string_view textureFilename,
		const XMVECTOR startPosition, const XMVECTOR endPosition, float destroyTimer = 0.f);

	//Lifetime being 0 means the emitter will loop
	ParticleEmitter* SpawnParticleEmitter(std::string textureFilename, XMVECTOR spawnPosition, float lifeTime = 0.f);

	void PlayAudioOneShot(const std::string audioFilename);

	void SaveGameWorldState();

	//Loads world at the end of the current frame. Call FileSystem::LoadWorld() if immediate loading is needed.
	void LoadWorldDeferred(std::string worldName);

	inline static const std::string gameInstanceSaveFile = "game_instance.sav";

	void SaveGameInstanceData();
	void LoadGameInstanceData();

	//Used with timers to be able to call LoadWorldAndMoveToEntranceTrigger() through Timer.
	extern std::string levelToMoveTo;
	extern std::string entranceTriggerTag;

	//Move to matching entrance trigger with same name as exited one in loaded world.
	void LoadWorldAndMoveToEntranceTrigger();

	void DisablePlayer();
};
