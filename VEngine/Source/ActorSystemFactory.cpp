#include "ActorSystemFactory.h"
#include "Debug.h"
#include "Actor.h"

std::unordered_map<size_t, ActorSystem*> *ActorSystemFactory::IDToSystemMap;
std::unordered_map<ActorSystem*, size_t> *ActorSystemFactory::systemToIDMap;
std::unordered_map<std::wstring, ActorSystem*> *ActorSystemFactory::nameToSystemMap;

//The currently selected actor system to spawn actors from.
ActorSystem* currentActiveActorSystem;

ActorSystemFactory::ActorSystemFactory()
{

}

size_t ActorSystemFactory::GetActorSystemID(ActorSystem* actorSystem)
{
	auto ID = systemToIDMap->find(actorSystem);
	return ID->second;
}

ActorSystem* ActorSystemFactory::GetActorSystem(size_t id)
{
	auto actorSystem = IDToSystemMap->find(id);
	return actorSystem->second;
}

ActorSystem* ActorSystemFactory::GetActorSystem(std::wstring name)
{
	auto actorSystem = nameToSystemMap->find(name);
	return actorSystem->second;
}

void ActorSystemFactory::GetAllActorSystems(std::vector<ActorSystem*>& actorSystems)
{
	for (auto& as : *IDToSystemMap)
	{
		actorSystems.push_back(as.second);
	}
}

void ActorSystemFactory::SetCurrentActiveActorSystem(ActorSystem* actorSystem)
{
	currentActiveActorSystem = actorSystem;
}

ActorSystem* ActorSystemFactory::GetCurrentActiveActorSystem()
{
	return currentActiveActorSystem;
}

void ActorSystemFactory::CreateActorSystem(const char* actorSystemName, const char* actorName, const char* superClass)
{
    //.cpp/.h "code-gen" for system and actor
    const char* actorSystemHeaderFile =
        "#pragma once\n"
        "\n"
        "#include \"Actor.h\"\n"
        "\n"
        "class AActor : public Actor\n"
        "{\n"
        "public:\n"
        "\tAActor();\n"
        "};\n"
        "\n"
        "class AActorSystem : public ActorSystem\n"
        "{\n"
        "public:\n"
        "\tvirtual void Tick(float deltaTime) override;\n"
        "\tvirtual void SpawnActors(int numToSpawn);\n"
        "\tvirtual void SpawnActor(Transform transform);\n"
        "};\n"
        "\n"
        "extern AActorSystem aActorSystem;\n";

    const char* actorSystemSourceFile =
        "#include \"AActor.h\"\n"
        "#include \"ActorSystemFactory.h\"\n"
        "\n"
        "AActorSystem aActorSystem;"
        "\n"
        "TestActorSystem::TestActorSystem()\n"
        "{\n"
        "\tshaderName = L\"shaders.hlsl\";\n"
        "\ttextureName = L\"texture2.jpg\";\n"
        "\tmodelName = \"cube.fbx\";\n"
        "\tname = L\"testactor\";\n"
        "\n"
        "\tsizeofActor = sizeof(TestActor);\n"
        "\n"
        "\tActorSystemFactory::Register<TestActorSystem>(this);\n"
        "}\n"
        "\n"
        "void TestActorSystem::Tick(float deltaTime)\n"
        "{\n"
        "\n"
        "}\n"
        "\n"
        "void TestActorSystem::SpawnActors(int numToSpawn)\n"
        "{\n"
        "\tInit<TestActor>(&gRenderSystem, numToSpawn);\n"
        "}\n"
        "\n"
        "void TestActorSystem::SpawnActor(Transform transform)\n"
        "{\n"
        "\tAddActor<TestActor>(transform);\n"
        "}\n"
        "\n";

    FILE* file;

    //Write .h file.
    fopen_s(&file, "AActor.h", "w");
    assert(file);
    fprintf(file, "%s", actorSystemHeaderFile);
    fflush(file);

    //Write .cpp file.
    fopen_s(&file, "AActor.cpp", "w");
    assert(file);
    fprintf(file, "%s", actorSystemSourceFile);
    fflush(file);

    fclose(file);
}
