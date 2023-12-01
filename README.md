# VEngine

_Note that this repository isn't for general use, it's more of an open source snapshot of an intermediate engine being developed alongside a game. Feel free to go through and take snippets of code and apply them to your own projects._

![image](https://github.com/sfederic/Engine/assets/45758254/ae28fc21-0288-45ae-bd82-1c61020f7221)

## Build/Usage Notes

_Helpful Fork [https://github.com/sfederic/Engine/pull/3] to build using premake._
_Thanks to https://github.com/ignite720_

* Visual Studio 2022 17.8
* Blender 3.4 

## Packages

DirectXTK (directxtk_desktop_win10) ver. 2021.8.2.1 - https://github.com/microsoft/DirectXTK

## Third Party Libraries:

* Physx ver 4.1.2 | Place PhysX headers/source into Code/Physx - https://github.com/NVIDIAGameWorks/PhysX (Easier to use https://vcpkg.io/en/index.html to download SDK [package name is physx:x64-windows])

* FBXSDK ver. 2020.1 | Place FBXSDK headers/source into Code/FbxSdk - https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-1

* DirectXMath | Spherical Harmonics (included in repo) https://github.com/microsoft/DirectXMath/tree/main/SHMath

* Qt ver. 5.15.0 - https://download.qt.io/official_releases/qt/5.15/

(The ImGui libs are already included in the repo)
* Dear ImGui - https://github.com/ocornut/imgui
* ImGuizmo - https://github.com/CedricGuillemet/ImGuizmo

## Helpful Extensions

* Qt VS Tools ver. 2.10.0 - https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022
* HLSL Tools for Visual Studio (Now an official part of Visual Studio) - https://marketplace.visualstudio.com/items?itemName=TimGJones.HLSLToolsforVisualStudio

## Overview

As this engine is more of a learning project for onlookers and myself, the techincal outset of the engine was to keep it simple and mostly in C++ with minimal use of other programming languages; that means usage of the C++ standard library and attempted usage of modern C++ concepts.

Below is a quick outline of various concepts the engine uses.

## Game Object Model

### Actors

The engine uses an Actor/Component model similar to Unity and Unreal. The inspiration came from Rare's presentation on Actor Ticks in UE4 in Sea of Thieves [https://www.youtube.com/watch?v=CBP5bpwkO54].

(A good starting reference on Game Object Models https://www.youtube.com/watch?v=jjEsB611kxs)

An inherited actor will be defined with a system via a macro.

```cpp
struct Enemy : Actor
{
    //essentially defines 'inline static ActorSystem<Enemy> system;'
    ACTOR_SYSTEM(Enemy);

    void Tick(float deltaTime) override;
};
```

### Actor Systems

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

### Worlds

A 'World' is the current view of the scene on-screen. Each World will have lists of the currently active Actor and Component systems.

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

## Actor and Component Properties 

For serialisiation, Actors and Components define their properties to be fetched at run-time.

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

`Properties` is a collection of `Property`s. VEngine uses the `typeid()` from `Properties::Add<T>()` to infer the type.

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

## Serialisation

Using `Properties`, serialisation infers property types by checking for a matching `typeid()`.

```cpp
void Serialiser::Serialise(Properties& properties)
{
    for (Property& property : properties)
    {
        if (property.type == typeid(int))
        {
            SerialiseInt<int>(property.value);    
        }
    }
}
```

## World Save File

Files with the `.vmap` extension describe the layout of a level in-game.
Below is an example of the text format to show how properties map to values.

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

VEngine uses DirectX 11 for 3D/2D rendering and DirectWrite + Direct2D for in-game UI.

## FBX Importing

VEngine uses the official FBX SDK to import models and animations. While not very robust, details can be gleamed in https://github.com/sfederic/VEngine/blob/7259295eb490466392d2f999852f023e92c7182a/VEngine/Code/Asset/FBXLoader.cpp

### Game UI

For in-game UI, an immediate approach is used. An example in-game widget would be declared like so:

```cpp
struct EnemyText : Widget
{
    std::string text;

    void Draw() override
    {
        Layout layout = AlignLayoutByScreenPercent(0.1, 0.9, 0.1, 0.2);
        DrawText(text, layout);
    }
}
```

### Physically Based Shading

While not a huge component of the engine, the implementations were taken from EA's Frostbite engine [https://www.ea.com/frostbite/news/moving-frostbite-to-pb].

### Global Illumination

VEngine uses a very simple Global Illumination technique using light probes spread around a level uniformly. Leveraging the DirectXSH spherical harmonics library (good reference for DirectX 11 [https://interplayoflight.wordpress.com/2021/12/31/occlusion-and-directionality-in-image-based-lighting-implementation-details/]), each probe takes a cubemap snapshot of its surroundings an encodes it using spherical harmonics. Actors then just find their closest probe and apply its colours per vertex normal.

References for this system were mainly taken from Bluepoint's Shadow of the Colossus [https://gdcvault.com/play/1027011/Advanced-Graphics-Summit-Lifting-the] and Sonic Unleashed [https://www.gdcvault.com/play/1428/Global-Illumination-in-SONIC].

### Skeletal Animation (FBX) with Blender Workflow

- Nice tutorial on the general gist of multiple animations from Blender to Unity [https://www.zuluonezero.net/2021/11/16/exporting-multiple-animations-from-blender-to-unity/]

To import skeletal .fbx animations via Blender:

* Add an Armature and bone structure to a mesh in Blender

![image](https://github.com/sfederic/VEngine/assets/45758254/44af7544-edcf-46d9-b14b-f09f3a13c5cc)

* hit Ctrl+P to parent the mesh to the armature and automate the weights

![image](https://github.com/sfederic/VEngine/assets/45758254/ef04d695-f06e-41f7-bc01-f6e7cc9545c7)

* Record the poses using the Timeline and Dope Sheet -> Action Editor windows

![image](https://github.com/sfederic/VEngine/assets/45758254/7a99555a-fdb9-4543-a80f-fd0a3ce49c48)

* Use the Non-Linear Animation window to organise animations

![image](https://github.com/sfederic/VEngine/assets/45758254/cbc73877-5679-45fa-a98b-e91fbc22105e)

* Export as an FBX with these settings, including all animations

![image](https://github.com/sfederic/VEngine/assets/45758254/7c0cf76c-bcf7-43e6-9255-615f8b7f69dd)

#### Setup Skeletal mesh in code

Note that a seperate .vmesh file of the mesh is needed without animations baked in (but with the same bone structure as the animations), assigned as the mesh of a SkeletalMeshComponent, where then the imported FBX animations are linked to.

For example:
```cpp
class AnimCube : public Actor
{
public:
	AnimCube()
    {
        skeletalMesh = CreateComponent("Skeleton", SkeletalMeshComponent("anim_cube.vmesh", "texture jpg"));
        rootComponent = skeletalMesh;
    }

    void Start() override
    {
        skeletalMesh->PlayAnimation("move");
    }

private:
	SkeletalMeshComponent* skeletalMesh = nullptr;
}
```
