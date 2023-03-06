#include "vpch.h"
#include "GameUtils.h"
#include <filesystem>
#include "Actors/Game/Player.h"
#include "Actors/Game/MapScreenPlayer.h"
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
#include "UI/Game/LevelCompleteWidget.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Particle/SpriteSheet.h"
#include "Particle/Polyboard.h"
#include "Asset/AssetFileExtensions.h"

namespace GameUtils
{
	std::string levelToMoveTo;

	void SetActiveCameraTarget(Actor* newTarget)
	{
		activeCamera->targetActor = newTarget;
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

		GameInstance::previousMapMovedFrom = levelToMoveTo;

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

	XMVECTOR RandomPointInTriggerNotContainedByMeshBounds(BoxTriggerComponent* boxTrigger)
	{
		//Keep vector of previous points to re-check them on Contains() == true below
		std::vector<XMVECTOR> previousContainedPoints;

		XMVECTOR point = boxTrigger->GetRandomPointInTriggerRounded();

		//Retry random point in bounds if it's inside another bounds
		for (auto& mesh : MeshComponent::system.GetComponents())
		{
			if (mesh->boundingBox.Contains(point))
			{
				previousContainedPoints.emplace_back(point);

				point = boxTrigger->GetRandomPointInTriggerRounded();
				for (auto& previousPoint : previousContainedPoints)
				{
					if (XMVector4Equal(point, previousPoint))
					{
						throw; //@Todo: think of something better here. e.g. if EVERY previous point is inside
						//mesh bounds, throw.
					}
				}
			}
		}

		return point;
	}

	void TriggerLevelComplete()
	{
		auto levelCompleteWidget = UISystem::CreateWidget<LevelCompleteWidget>();
		UISystem::SetWidgetControlActive(true);
		levelCompleteWidget->AddToViewport();
	}

	void SavePlayerMapScreenPos()
	{
		GameInstance::SetGlobalProp("PlayerMapScreenPos", MapScreenPlayer::system.GetFirstActor()->GetPosition());
	}
}
