#include "ActorSystemFactory.h"
#include "Debug.h"
#include "Actor.h"
#include <typeindex>

std::unordered_map<size_t, ActorSystem*> *ActorSystemFactory::IDToSystemMap;
std::unordered_map<ActorSystem*, size_t> *ActorSystemFactory::systemToIDMap;
std::unordered_map<std::wstring, ActorSystem*> *ActorSystemFactory::nameToSystemMap;
std::unordered_map<std::type_index, ActorSystem*>* ActorSystemFactory::actorTypeToSystemMap;

//The currently selected actor system to spawn actors from.
ActorSystem* currentActiveActorSystem;

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

ActorSystem* ActorSystemFactory::GetActorSystem(std::type_index actorType)
{
    auto actorSystemIt = actorTypeToSystemMap->find(typeid(actorType));
    if (actorSystemIt != actorTypeToSystemMap->end())
    {
        return actorSystemIt->second;
    }

    return nullptr;
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
    char actorSystemHeaderFile[1024];
    snprintf(actorSystemHeaderFile, 1024,
        "#pragma once\n"
        "\n"
        "#include \"Actor.h\"\n"
        "\n"
        "class %s : public Actor\n"
        "{\n"
        "public:\n"
        "\%s();\n"
        "};\n"
        "\n"
        "class %s : public ActorSystem\n"
        "{\n"
        "public:\n"
        "\tvirtual void Tick(float deltaTime) override;\n"
        "\tvirtual void SpawnActors(int numToSpawn);\n"
        "\tvirtual void SpawnActor(Transform transform);\n"
        "};\n"
        "\n"
        "extern %s %s;\n",
        actorName, actorName, actorSystemName, actorSystemName, actorSystemName);


    char actorSystemSourceFile[1024];
    snprintf(actorSystemSourceFile, 1024,
        "#include \"%s.h\"\n"
        "#include \"ActorSystemFactory.h\"\n"
        "\n"
        "%s %s;\n"
        "\n"
        "%s::%s()\n"
        "{\n"
        "\tshaderName = L\"shaders.hlsl\";\n"
        "\ttextureName = L\"texture2.jpg\";\n"
        "\tmodelName = \"cube.fbx\";\n"
        "\tname = L\"testactor\";\n"
        "\n"
        "\tsizeofActor = sizeof(%s);\n"
        "\n"
        "\%s::Register<%s>(this);\n"
        "}\n"
        "\n"
        "void %s::Tick(float deltaTime)\n"
        "{\n"
        "\n"
        "}\n"
        "\n"
        "void %s::SpawnActors(int numToSpawn)\n"
        "{\n"
        "\tInit<%s>(numToSpawn);\n"
        "}\n"
        "\n"
        "void %s::SpawnActor(Transform transform)\n"
        "{\n"
        "\tAddActor<%s>(transform);\n"
        "}\n"
        "\n",
        actorSystemName, actorSystemName, actorSystemName, actorSystemName, actorSystemName,
        actorName, actorSystemName, actorSystemName, actorSystemName, actorSystemName,
        actorName, actorSystemName, actorName);

    FILE* file;

    //Write .h file.
    std::string fileName = actorSystemName;
    fileName += ".h";

    fopen_s(&file, fileName.c_str(), "w");
    assert(file);
    fprintf(file, "%s", actorSystemHeaderFile);
    fflush(file);

    //Write .cpp file.
    fileName = actorSystemName;
    fileName += ".cpp";

    fopen_s(&file, fileName.c_str(), "w");
    assert(file);
    fprintf(file, "%s", actorSystemSourceFile);
    fflush(file);

    fclose(file);
}
