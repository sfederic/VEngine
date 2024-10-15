#include "vpch.h"
#include "Localisation.h"
#include "Core/Log.h"
#include <qfile.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

std::string gLanguage = "EN";

std::wstring Localise::GetString(const std::string& key)
{
	QFile file("Dialogue/test.json");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
	{
		Log("Json file not found for localisation. Key: [%s]", key.c_str());
		return L"NOT_FOUND";
	}

	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

	//Get the root JSON element in the file
	QJsonObject rootObject = doc.object();
	
	//Find object the matching key
	QJsonArray stringArray = rootObject.value(key.c_str()).toArray();

	QJsonObject stringObject = stringArray[0].toObject();
	const std::wstring ouputValue = stringObject.value(gLanguage.c_str()).toString().toStdWString();
	return ouputValue;
}

void Localise::SetLanguage(const std::string_view language)
{
	gLanguage = language;
}
