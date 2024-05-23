#pragma once

#include "Core/UID.h"
#include <string>
#include <optional>
#include <functional>
#include <typeindex>

struct Property
{
	// @Todo: there's something I want to try with the 'change' function. Since most actors have the direct type
	// of the property (e.g. bool isHot;), if you could wrap all properties on an actor in some class, what you could
	// do is call the Change() function every time the variable is set/get, then you wouldn't have to do weird shit
	// in Tick() where you're constantly looping over bools.

	//Function to call when the set value is changed.
	std::function<void(void*)> change;

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
