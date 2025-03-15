export module Components.IComponentSystem;

import <vector>;
import <string>;

import Core.SystemStates;

class Component;
class Actor;
class Serialiser;
class BinarySerialiser;
class Deserialiser;
class BinaryDeserialiser;

export class IComponentSystem
{
public:
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void Start() = 0;
	virtual void DestroyAll() = 0;
	virtual void Cleanup() = 0;
	virtual void Serialise(Serialiser& s) = 0;
	virtual void SerialiseBinary(BinarySerialiser& s) = 0;
	virtual void Deserialise(Deserialiser& s) = 0;
	virtual void DeserialiseBinary(BinaryDeserialiser& d) = 0;
	virtual Component* SpawnComponent(Actor* owner) = 0;
	virtual std::vector<Component*> GetComponentsAsBaseClass() = 0;
	virtual size_t GetNumComponents() = 0;
	virtual bool Empty() = 0;
	virtual Component* FindComponentByName(std::string componentName) = 0;

	auto GetName() { return _name; }

protected:
	SystemStates systemState = SystemStates::Unloaded;
	std::string _name;
};
