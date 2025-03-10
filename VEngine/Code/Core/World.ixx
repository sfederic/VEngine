export module Core.World;

import <vector>;
import <string>;
import <unordered_map>;

import Core.UID;

export class IActorSystem;
export class IComponentSystem;
export class Component;
export class Actor;

export class World
{
public:
	static World& Get()
	{
		static World instance;
		return instance;
	}

	void Init();

	//Called on level load
	void Start();

	//Called when gameplay begins
	void WakeAndStartAllActors();
	void StartAllComponents();

	//Call End() on all actors on gameplay end
	void EndAllActors();

	void DestroyAllDeferredActors();

	//Create default starting actors for a map.
	void CreateDefaultMapActors();

	std::vector<IActorSystem*> GetLayerActorSystems();

	void TickAllActorSystems(float deltaTime);
	void TickAllComponentSystems(float deltaTime);
	std::vector<Actor*> GetAllActorsInWorld();

	Actor* GetActorByUID(UID uid);
	Actor* GetActorByUIDAllowNull(UID uid);
	Actor* GetActorByName(std::string actorName);
	Actor* GetActorByNameAllowNull(std::string actorName);

	std::vector<Component*> GetAllComponentsInWorld();

	void AddActorToWorld(Actor* actor);

	void RemoveActorFromWorld(Actor* actor);
	void RemoveActorFromWorld(UID actorUID);
	void RemoveActorFromWorld(std::string actorName);

	void ClearAllActorsFromWorld();
	void DestroyAllActorsAndComponentsInWorld();

	bool CheckIfActorExistsInWorld(std::string actorName);
	bool CheckIfActorExistsInWorld(UID actorUID);

	void Cleanup();

	template <typename T>
	std::vector<T*> GetAllActorsOfTypeInWorld()
	{
		auto actorSystem = ActorSystemCache::Get().GetSystem(typeid(T));
		auto actors = actorSystem->GetActorsAsBaseClass();
		std::vector<T*> outActors;
		for (auto actor : actors)
		{
			outActors.emplace_back(dynamic_cast<T*>(actor));
		}
		return outActors;
	}

	template <typename T>
	std::vector<T*> GetAllComponentsOfType()
	{
		std::vector<T*> outComponents;
		auto components = GetAllComponentsInWorld();
		for (auto component : components)
		{
			auto componentType = dynamic_cast<T*>(component);
			if (componentType)
			{
				outComponents.emplace_back(componentType);
			}
		}
		return outComponents;
	}

	template <typename T>
	std::vector<T*> GetAllComponentSystemsOfType()
	{
		std::vector<T*> outComponentSystems;

		for (auto componentSystem : activeComponentSystems)
		{
			auto systemType = dynamic_cast<T*>(componentSystem);
			if (systemType)
			{
				outComponentSystems.emplace_back(systemType);
			}
		}

		return outComponentSystems;
	}

	template <typename T>
	std::vector<Actor*> GetAllActorsOfTypeAsActor()
	{
		std::vector<Actor*> outActors;

		auto actors = GetAllActorsInWorld();
		for (auto actor : actors)
		{
			if (dynamic_cast<T*>(actor))
			{
				outActors.emplace_back(actor);
			}
		}

		return outActors;
	}

	template <typename T>
	std::vector<T*> GetAllActorsAsBaseType()
	{
		std::vector<T*> outActors;
		for (auto actor : World::GetAllActorsInWorld())
		{
			auto actorType = dynamic_cast<T*>(actor);
			if (actorType)
			{
				outActors.emplace_back(actorType);
			}
		}
		return outActors;
	}

	//This function is more for actors that need to find an actor in-world
	template <typename T>
	T* GetActorByNameAndLogCast(std::string actorName)
	{
		auto foundActorIt = actorNameMap.find(actorName);
		if (foundActorIt == actorNameMap.end())
		{
			Log("[%s] actor not found.", actorName.c_str());
			return nullptr;
		}

		auto castResult = dynamic_cast<T*>(foundActorIt->second);
		if (castResult == nullptr)
		{
			Log("[%s] actor not of type [%s]. Unable to cast.", actorName.c_str(), typeid(T).name());
			return nullptr;
		}
		return castResult;
	}

	template <typename T>
	void RemoveAllActorsOfType()
	{
		auto actors = GetAllActorsInWorld();
		for (auto actor : actors)
		{
			if (dynamic_cast<T*>(actor))
			{
				auto actorSystem = actor->GetActorSystem();
				actorSystem->RemoveInterfaceActor(actor);
			}
		}
	}

	auto GetWorldFilename() { return worldFilename; }
	void SetWorldFilename(std::string name) { worldFilename = name; }

	auto GetActiveActorSystems() { return activeActorSystems; }
	auto GetActiveComponentSystems() { return activeComponentSystems; }

private:
	std::string worldFilename;

	std::unordered_map<UID, Actor*> actorUIDMap;
	std::unordered_map<std::string, Actor*> actorNameMap;

	std::vector<IActorSystem*> activeActorSystems;
	std::vector<IComponentSystem*> activeComponentSystems;
};
