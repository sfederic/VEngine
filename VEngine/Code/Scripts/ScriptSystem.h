#pragma once
#include <map>
#include <functional>

struct Script;

//Acts as a factory to create scripts via their name by holding template instances of each of their
//types, mapped to each script class name.
class ScriptSystem
{
private:
    std::map<std::string, std::function<Script*()>> scriptNameToCreateFunctionMap;

public:
    ScriptSystem();
    Script* CreateScript(std::string name);

private:
    template <typename ScriptType>
    void AddScript(std::string name, std::function<ScriptType*()> func)
    {
        static_assert(std::is_base_of<Script, ScriptType>::value);
        scriptNameToCreateFunctionMap.emplace(name, func);
    }
};
