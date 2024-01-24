#pragma once

#include <string>

//File extensions for engine specific formats
struct AssetFileExtensions
{
	inline static const std::string mesh = ".vmesh";
	inline static const std::string actorTemplate = ".actor";
	inline static const std::string dialogue = ".dialog";
	inline static const std::string journalEntry = ".journal";
	inline static const std::string cutscene = ".cutscene";
	inline static const std::string worldMap = ".vmap";
	inline static const std::string gameSave = ".vmap";
	inline static const std::string material = ".vmat";
};
