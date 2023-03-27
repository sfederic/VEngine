#include "vpch.h"
#include "GameUtils.h"
#include <filesystem>
#include "Actors/Game/Player.h"
#include "Audio/AudioSystem.h"
#include "Core/World.h"
#include "Core/FileSystem.h"
#include "GameInstance.h"
#include "Core/Camera.h"
#include "Components/CameraComponent.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "UI/UISystem.h"
#include "UI/ScreenFadeWidget.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Particle/SpriteSheet.h"
#include "Particle/Polyboard.h"
#include "Asset/AssetFileExtensions.h"

namespace GameUtils
{
	std::string levelToMoveTo;

	bool CheckIfMemoryExists(const std::string memoryName)
	{
		auto memIt = GameInstance::playerMemories.find(memoryName);
		return memIt != GameInstance::playerMemories.end();
	}

	void SetActiveCameraTarget(Actor* newTarget)
	{
		activeCamera->targetActor = newTarget;
	}

	void SetActiveCameraTargetAndZoomIn(Actor* newTarget)
	{
		activeCamera->targetActor = newTarget;
		Player::system.GetFirstActor()->nextCameraFOV = 30.f;
	}

	void SetActiveCameraTargetAndZoomOut(Actor* newTarget)
	{
		activeCamera->targetActor = newTarget;
		Player::system.GetFirstActor()->nextCameraFOV = 60.f;
	}

	void SetCameraBackToPlayer()
	{
		auto player = Player::system.GetFirstActor();
		player->nextCameraFOV = 60.f;
		activeCamera->targetActor = player;
	}

	void CameraShake(float shake)
	{
		activeCamera->shakeLevel = shake;
	}

	SpriteSheet* SpawnSpriteSheet(std::string textureFilename, XMVECTOR spawnPosition, bool loop, int numRows, int numColumns)
	{
		auto spriteSheet = SpriteSheet::system.Add("SpriteSheet", nullptr, SpriteSheet(), false);

		spriteSheet->SetLocalPosition(spawnPosition);
		spriteSheet->textureData.filename = textureFilename;
		spriteSheet->loopAnimation = loop;
		spriteSheet->numSheetRows = numRows;
		spriteSheet->numSheetColumns = numColumns;

		spriteSheet->Create();

		return spriteSheet;
	}

	Polyboard* SpawnPolyboard(const std::string_view textureFilename,
		const XMVECTOR startPosition, const XMVECTOR endPosition, float destroyTimer)
	{
		std::string name = "Polyboard" + std::to_string(Polyboard::system.GetNumComponents());
		auto polyboard = Polyboard::system.Add(name, nullptr, Polyboard(), true);

		polyboard->SetStartPoint(startPosition);
		polyboard->SetEndPoint(endPosition);

		if (destroyTimer > 0.f)
		{
			polyboard->SetDestroyTimer(destroyTimer);
		}

		return polyboard;
	}

	void PlayAudioOneShot(const std::string audioFilename)
	{
		AudioSystem::PlayAudio(audioFilename);
	}

	void SaveGameWorldState()
	{
		//The deal with save files here is the the .vmap file is the original state of the world seen in-editor
		//and the .sav version of it is based on in-game player saves. So if a .sav version exists, that is loaded
		//instead of the .vmap file, during gameplay.

		auto firstOf = World::worldFilename.find_first_of(".");
		std::string str = World::worldFilename.substr(0, firstOf);
		std::string file = str += AssetFileExtensions::gameSave;

		World::worldFilename = file;
		FileSystem::SerialiseAllSystems();
	}

	void LoadWorldDeferred(std::string worldName)
	{
		std::string path = "WorldMaps/" + worldName;
		FileSystem::SetDeferredWorldLoad(worldName);
	}

	void SaveGameInstanceData()
	{
		Properties instanceProps = GameInstance::GetGlobalProps();
		Serialiser s(gameInstanceSaveFile, OpenMode::Out);
		s.Serialise(instanceProps);
	}

	void LoadGameInstanceData()
	{
		Properties instanceProps = GameInstance::GetGlobalProps();
		Deserialiser d(gameInstanceSaveFile, OpenMode::In);
		d.Deserialise(instanceProps);
	}

	void LoadWorldAndMoveToEntranceTrigger()
	{
		if (levelToMoveTo.empty())
		{
			return;
		}

		LoadWorldDeferred(levelToMoveTo);

		Input::blockInput = false;
	}

	void DisablePlayer()
	{
		auto player = Player::system.GetFirstActor();
		player->SetActive(false);
	}

	void SetActiveCamera(CameraComponent* camera)
	{
		activeCamera = camera;
	}
}
