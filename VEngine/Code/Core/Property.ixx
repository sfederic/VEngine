export module Core.Property;

import <functional>;
import <string>;
import <optional>;
import <typeindex>;

import Core.UID;

export struct Property
{
	//Function to call when the set value is changed.
	std::function<void(Property&)> change;

	std::string name;

	//Path to take filenames from when working with autocomplete on stringwidgets.
	//Need to pass in paths with leading & trailing slashes. Eg. "/Dialogues/"
	std::string autoCompletePath;

	std::optional<std::type_index> info;

	uint64_t size = 0;

	void* data = nullptr;

	UID ownerUID = 0;

	//Sets widgets to be inactive/readonly
	bool readOnly = false;

	//hide property in UI and skips over copying properties across to new props
	bool hide = false;

	bool useActorsAutoComplete = false;

	template <typename T>
	T* GetData()
	{
		return (T*)data;
	}
};

// @Todo: there's something I want to try with the 'change' function. Since most actors have the direct type
// of the property (e.g. bool isHot;), if you could wrap all properties on an actor in some class, what you could
// do is call the Change() function every time the variable is set/get, then you wouldn't have to do weird shit
// in Tick() where you're constantly looping over bools. It's too much to implement right now, but it does work
// and needs a bit of heavy reworking with Properties. Below is basically how it would look.

//template <typename T>
//class PropertyWrapper
//{
//private:
//	T _data;
//	Property _prop;
//
//public:
//	PropertyWrapper(T value) : _data(value) {}
//
//	T& Get() { return _data; }
//
//	void SetChange(std::function<void(Property&)> func)
//	{
//		_prop.change = func;
//	}
//
//	void Set(T value)
//	{
//		_data = value;
//		if (_prop.change)
//		{
//			_prop.change(_prop);
//		}
//	}
//};
