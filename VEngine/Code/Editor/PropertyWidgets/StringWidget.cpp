#include "vpch.h"
#include "StringWidget.h"
#include <filesystem>
#include <qcompleter.h>
#include "Core/World.h"

StringWidget::StringWidget(Property& value_)
{
	prop = value_;

	value = value_.GetData<std::string>();
	setText(QString::fromStdString(value->data()));

	SetDirectoryFilenamesAutoComplete(prop.autoCompletePath);
	SetActorWorldListAutoComplete();

	connect(this, &QLineEdit::editingFinished, this, &StringWidget::SetValue);
}

void StringWidget::SetValue()
{
	const std::string txt = text().toStdString();

	if (txt == *value)
	{
		IPropertyWidget::SetValue<std::string>(prop);
	}

	value->assign(txt);

	if (prop.change)
	{
		prop.change(value);
	}

	clearFocus();
}

void StringWidget::ResetValue()
{
	if (value)
	{
		setText(QString::fromStdString(value->data()));
	}
}

void StringWidget::SetDirectoryFilenamesAutoComplete(const std::string& directoryPath)
{
	if (directoryPath.empty())
	{
		return;
	}

	//Can only use actors or filenames for now with auto-complete.
	assert(prop.useActorsAutoComplete == false);

	const std::string path = std::filesystem::current_path().generic_string() + prop.autoCompletePath;

	QStringList dirContents;

	for (auto const& entry : std::filesystem::recursive_directory_iterator(path))
	{
		//Use generic_string() here. Windows likes to throw in '\\' when it wants with string().
		const std::string entryPath = entry.path().generic_string();

		//Grab the index so file paths are displayed correctly on autocomplete. Eg. "test_map/item.png"
		size_t index = entryPath.find(prop.autoCompletePath);
		std::string file = entryPath.substr(index + prop.autoCompletePath.size());

		dirContents.append(QString::fromStdString(file));
	}

	QCompleter* fileEditCompleter = new QCompleter(dirContents, this);
	fileEditCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	fileEditCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
	this->setCompleter(fileEditCompleter);
}

void StringWidget::SetActorWorldListAutoComplete()
{
	if (!prop.useActorsAutoComplete)
	{
		return;
	}

	//Can only use actors or filenames for now with auto-complete.
	assert(prop.autoCompletePath.empty() == true);

	QStringList actorNameList;
	for (Actor* actor : World::GetAllActorsInWorld())
	{
		std::string actorName = actor->GetName();
		actorNameList.append(QString::fromStdString(actorName));
	}

	QCompleter* fileEditCompleter = new QCompleter(actorNameList, this);
	fileEditCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	fileEditCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
	this->setCompleter(fileEditCompleter);
}
