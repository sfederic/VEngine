# VEngine

![image](https://github.com/sfederic/Engine/assets/45758254/ae28fc21-0288-45ae-bd82-1c61020f7221)

## Build Notes

Premake5 is required. Download the linked release and add it as a PATH environment variable.
https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-windows.zip

Put this into D:\dev2
git -C D:\ clone https://github.com/ignite720/dev2.git

Then run `premake5-generate.bat`, all projects will be generated in the `build` directory.
`premake5-clean.bat` removes all generated content in the bin and build directories.

_Thanks to #ignite720_

* Visual Studio 2022 17.5
* Blender 3.4 (For FBX SDK)

## Packages

DirectXTK (directxtk_desktop_win10) ver. 2021.8.2.1 - https://github.com/microsoft/DirectXTK

## Third party libs:

* Physx ver 4.1.2 | Throw PhysX headers/source into Code/Physx - https://github.com/NVIDIAGameWorks/PhysX (used https://vcpkg.io/en/index.html to download SDK [package name is physx:x64-windows])

* FBXSDK ver. 2020.1 | Throw FBXSDK headers/source into Code/FbxSdk - https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-1

* DirectXMath | Spherical Harmonics (included in repo) https://github.com/microsoft/DirectXMath/tree/main/SHMath

* Qt ver. 5.15.0 - https://download.qt.io/official_releases/qt/5.15/

(The ImGui libs are already included in the repo)
* Dear ImGui - https://github.com/ocornut/imgui
* ImGuizmo - https://github.com/CedricGuillemet/ImGuizmo

## Helpful Extensions

* Qt VS Tools ver. 2.10.0 - https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022
* HLSL Tools for Visual Studio - https://marketplace.visualstudio.com/items?itemName=TimGJones.HLSLToolsforVisualStudio

  As the engine is more of a learning project for onlookers and myself, the techincal outset of the engine was to keep it simple and mostly in C++ with, minimal use of outisde programming languages; that means usage of the C++ standard library and attempted usage of modern C++ concepts. This also extends to the rendering component of the engine, which are also simple.

Below is a quick outline of various concepts the engine uses. (Note that the code here is for illustration purposes.)

### Game Object Model

The engine uses an Actor/Component model similar to Unity and Unreal. The inspiration came from Rare's presentation on Actor Ticks in UE4 in Sea of Thieves. 

An inherited actor will be defined with a system via a macro.

```cpp
struct Enemy : Actor
{
    //essentially defines 'inline static ActorSystem<Enemy> system;'
    ACTOR_SYSTEM(Enemy);

    void Tick(float deltaTime) override;
};
```

Both Actors and Components have Systems. A system essentially looks like:

```cpp
template <typename ActorType>
struct ActorSystem : IActorSystem
{
    std::vector<ActorType*> actors;

    void Tick(float deltaTime) override
    {
        for(auto actor : actors)
        {
            actor->Tick(deltaTime);
        }
    }
}
```

A 'World' is the current view of the scene on-screen. Each World will have a list of the currently active Actor and Component systems.

```cpp
struct World
{
    std::vector<IActorSystem*> actorSystems;
    std::vector<IComponentSystem*> componentSystems;

    void Tick(float deltaTime)
    {
        TickAllActorSystems(deltaTime);
        TickAllComponentSystems(deltaTime)
    }
}
```

### Actor and Component Properties 

To deal with serialising out fields, Actors and Components define their properties to be fetched at run-time.

```cpp
struct Enemy : Actor
{
    int attack = 5;
    
    Properties GetProps() override
    {
        Properties props = Actor::GetProps();
        props.Add<int>("Attack", &attack);
        return props;
    }
}
```

`Properties` is a collection of `Property`s. The engine uses the `typeid()` from `Properties::Add<T>()` to infer the type.

```cpp
struct Property
{
    std::optional<std::type_info> type;
    void* value;
}

struct Properties
{
    std::map<std::string, Property> propertyMap;

    template <typename T>
    void Add(std::string propertyName, T* propertyValue)
    {
        Property property;
        property.type = typeid(T);
        property.value = propertyValue;

        propertyMap.emplace(propertyName, property);
    }
}

```

### Serialisation

Using the above `Properties`, serialisation infers property types by checking for a matching `typeid()`.

```cpp
void Serialiser::Serialise(Properties& properties)
{
    for(Property& property : properties)
    {
        if(property.type == typeid(int))
        {
            SerialiseInt<int>(property.value);    
        }
    }
}
```

### World Save File

Files with the `.vmap` extension describe the layout of a level in-game.
There are binary and text-based formats. Below is an example of the text format to show how properties map to values.

```
Enemy  //This type name fetches the appropriate system (in this case an Actor class of 'Enemy').
13     //This is how many instances of the class to create.
Attack //Property name.
16     //Property value.
next   //Move onto the next Enemy instance...
Attack
3
```

## Rendering

VEngine uses DirectX11 for 3D/2D rendering and DirectWrite + Direct2D for in-game UI.

### 3D Renderer

- Forward Renderer
- Light Probes
- Shadow Mapping

### In-Game UI

TODO - rest of the Readme

