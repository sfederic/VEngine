export module Components.ComponentSystemDefine;

import Components.ComponentSystem;

export template<typename T>
class ComponentSystemDefine
{
public:
	inline static ComponentSystem<T> system;

	void Remove()
	{
		system.Remove(GetIndex());
	}

	virtual int GetIndex() const { return 0; }
};
