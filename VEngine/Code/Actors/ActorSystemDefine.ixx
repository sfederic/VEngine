export module ActorSystemDefine;

import Actors.ActorSystem;

export template<typename T>
class ActorSystemDefine
{
public:
	inline static ActorSystem<T> system;

	void Remove()
	{
		system.Remove(GetIndex());
	}

	virtual int GetIndex() const { return 0; }
};
