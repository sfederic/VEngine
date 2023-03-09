#pragma once

#include "Core/UID.h"
#include <string>
#include <optional>
#include <functional>
#include <typeindex>

struct Property
{
	void* data = nullptr;

	UID ownerUID = 0;

	uint64_t size = 0;

	std::optional<std::type_index> info;

	std::string name;

	//Path to take filenames from when working with autocomplete on stringwidgets.
	//Need to pass in paths with leading & trailing slashes. Eg. "/Dialogues/"
	std::string autoCompletePath;

	//Sets widgets to be inactive/readonly
	bool readOnly = false;

	//hide property in UI
	bool hide = false;

	std::function<void(void*)> change;

	template <typename T>
	T* GetData()
	{
		return (T*)data;
	}
};
