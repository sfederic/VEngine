#include "vpch.h"
#include "GameUtils.h"
#include "Actors/Game/Player.h"
#include "Audio/AudioSystem.h"
#include "Core/World.h"
#include "Core/FileSystem.h"
#include "GameInstance.h"
#include "Core/Input.h"
#include "Particle/SpriteSheet.h"
#include "Particle/ParticleEmitter.h"
#include "Particle/Polyboard.h"
#include "Asset/AssetFileExtensions.h"
#include "Components/MeshComponent.h"

namespace GameUtils
{
	std::string levelToMoveTo;
	std::string entranceTriggerTag;

	SpriteSheet& SpawnSpriteSheet(std::string_view textureFilename, XMVECTOR spawnPosition, bool loop, int numRows, int numColumns, float animationSpeed)
	{
		auto spriteSheet = SpriteSheet::system.Add("SpriteSheet", nullptr, SpriteSheet(), false);

		spriteSheet->SetLocalPosition(spawnPosition);
		spriteSheet->SetupSpriteData(textureFilename, numRows, numColumns, loop);
		spriteSheet->Create();
		spriteSheet->SetAnimationSpeed(animationSpeed);

		return *spriteSheet;
	}

	Polyboard& SpawnPolyboard(const std::string_view textureFilename,
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

		return *polyboard;
	}

	ParticleEmitter* SpawnParticleEmitter(std::string textureFilename, XMVECTOR spawnPosition, float lifeTime)
	{
		auto emitter = ParticleEmitter::system.Add("SpawnedEmitter", nullptr, ParticleEmitter(textureFilename), true);
		emitter->SetWorldPosition(spawnPosition);
		emitter->emitterLifetime = lifeTime;
		return emitter;
	}

	void PlayAudioOneShot(const std::string audioFilename)
	{
		auto channelID = AudioSystem::LoadAudio(audioFilename);
		AudioSystem::PlayAudio(channelID);
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

		if (GameInstance::useGameSaves)
		{
			FileSystem::SerialiseAllSystems();
		}

		LoadWorldDeferred(levelToMoveTo);

		Input::blockInput = false;
	}

	void DisablePlayer()
	{
		auto player = Player::system.GetFirstActor();
		player->SetActive(false);
	}

	void SetLinkedMeshEffect(MeshComponent* mesh)
	{
		mesh->SetTexture("UI/spellbinding_circle.png");
		mesh->SetUVOffsetSpeed(XMFLOAT2(0.075f, 0.05f));
		mesh->SetUVRotationSpeed(0.05f);
	}
}
