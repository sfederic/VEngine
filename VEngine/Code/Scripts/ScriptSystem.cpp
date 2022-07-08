#include "vpch.h"
#include "ScriptSystem.h"
#include "Log.h"

ScriptSystem::ScriptSystem()
{
#define ADD_SCRIPT(x) AddScript<x>(#x, &CreateScript<x>);

    //Place scripts here by typename
    //E.g. ADD_SCRIPT(TestScript);
}

Script* ScriptSystem::CreateScript(std::string name)
{
    auto scriptIt = scriptNameToCreateFunctionMap.find(name);
    if (scriptIt == scriptNameToCreateFunctionMap.end())
    {
        Log("Script [%s] not found on creating script.", name.c_str());
        return nullptr;
    }

    return scriptNameToCreateFunctionMap[name]();
}
