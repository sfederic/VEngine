#include "FileSystem.h"
#include "World.h"
#include "Serialiser.h"
#include "Actors/IActorSystem.h"

FileSystem fileSystem;

void FileSystem::WriteAllActorSystems(std::string worldName)
{
	std::string file = "WorldMaps/" + worldName;
	file += ".sav";

	Serialiser s(file, std::ios_base::out);
	std::ostream os(&s.fb);

	for (IActorSystem* actorSystem : world.activeActorSystems)
	{
		actorSystem->Serialise(os);
	}
}
