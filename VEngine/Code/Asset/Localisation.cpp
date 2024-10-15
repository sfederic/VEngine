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

	const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	const QJsonObject rootObject = doc.object();
	const QJsonObject foundObject = rootObject.value(key.c_str()).toObject();
	
	const std::wstring ouputValue = foundObject.value(gLanguage.c_str()).toString().toStdWString();
	return ouputValue;
}

void Localise::SetLanguage(const std::string_view language)
{
	gLanguage = language;
}
