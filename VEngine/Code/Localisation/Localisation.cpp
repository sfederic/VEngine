#include "vpch.h"
#include "Localisation.h"
#include "Core/Log.h"
#include <qfile.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include "Asset/AssetBaseFolders.h"

std::string gLanguage = Locales::english;

std::unordered_map<std::string, std::wstring> gLocalisedStringsCache;

std::wstring Localise::GetString(const std::string& key, const std::string& filename)
{
	auto stringCacheIt = gLocalisedStringsCache.find(key);
	if (stringCacheIt != gLocalisedStringsCache.end())
	{
		return stringCacheIt->second;
	}

	const std::string filePath = AssetBaseFolders::dialogue + filename;
	QFile file(filePath.c_str());

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
	{
		Log("Json file [%s] not found for localisation", filePath.c_str());
		return L"JSON_FILE_NOT_FOUND";
	}

	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

	//Get the root JSON element in the file
	QJsonObject rootObject = doc.object();

	//Find object with matching key
	if (!rootObject.contains(key.c_str()))
	{
		Log("Localisation key [%s] not found in file [%s]", key.c_str(), filePath.c_str());
		return L"LOCALISATION_KEY_NOT_FOUND";
	}
	QJsonArray stringArray = rootObject.value(key.c_str()).toArray();

	//Make sure the language entry exists for this string
	QJsonObject stringObject = stringArray[0].toObject();
	if (!stringObject.contains(gLanguage.c_str()))
	{
		Log("Language key [%s] not found for in file [%s] in key [%s]", gLanguage.c_str(), filePath.c_str(), key.c_str());
		return L"LANGUAGE_KEY_NOT_FOUND";
	}

	std::wstring ouputValue = stringObject.value(gLanguage.c_str()).toString().toStdWString();

	gLocalisedStringsCache.emplace(key, ouputValue);

	file.close();

	return ouputValue;
}

void Localise::SetLanguage(const std::string_view language)
{
	gLanguage = language;
	Log("Locale set to [%s]", language.data());
	
	//Clear out the string cache because if locales are changed, keys will be fetching 
	//previous values from the previously set locale.
	gLocalisedStringsCache.clear();
}
