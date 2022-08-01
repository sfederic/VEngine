#include "vpch.h"
#include "DialogueDock.h"
#include <fstream>
#include <qtreewidget.h>
#include <qstandarditemmodel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qfiledialog.h>
#include <qcombobox.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include "World.h"
#include "Actors/Actor.h"
#include "Gameplay/ConditionSystem.h"
#include "VFunctionSystem.h"
#include "VString.h"

//columns for QTreeWidget
const int lineColumn = 0;
const int gotoColumn = 1;
const int actorColumn = 2;
const int conditionColumn = 3;
const int conditionArgColumn = 4;
const int textColumn = 5;

DialogueDock::DialogueDock() : QDockWidget("Dialogue")
{
	//TREE WIDGET
	dialogueTree = new QTreeWidget(); 
	dialogueTree->setColumnCount(6);
	dialogueTree->setHeaderLabels({ "Line", "Goto", "Actor", "Condition", "CondArg", "Text"});

	//BUTTONS
	auto insertDialogueLineButton = new QPushButton("Insert Line");
	connect(insertDialogueLineButton, &QPushButton::clicked, this, &DialogueDock::AddEmptyDialogueLine);

	auto deleteLineButton = new QPushButton("Delete");
	connect(deleteLineButton, &QPushButton::clicked, this, &DialogueDock::DeleteLine);

	auto saveButton = new QPushButton("Save");
	connect(saveButton, &QPushButton::clicked, this, &DialogueDock::SaveDialogueToFile);

	auto loadButton = new QPushButton("Load");
	connect(loadButton, &QPushButton::clicked, this, &DialogueDock::LoadDialogueFile);

	//BUTTONS HORIZONTAL LAYOUT
	auto hLayout = new QHBoxLayout();
	hLayout->addWidget(insertDialogueLineButton);
	hLayout->addWidget(deleteLineButton);
	hLayout->addWidget(saveButton);
	hLayout->addWidget(loadButton);

	//WIDGET LAYOUT
	auto vLayout = new QVBoxLayout();
	vLayout->addLayout(hLayout);
	vLayout->addWidget(dialogueTree);

	auto dialogueWidget = new QWidget();
	dialogueWidget->setLayout(vLayout);

	setWidget(dialogueWidget);
}

void DialogueDock::PopulateTreeItem(QTreeWidgetItem* item)
{
	int index = dialogueTree->indexOfTopLevelItem(item);
	item->setText(0, QString::number(index));
	item->setFlags(item->flags() | Qt::ItemIsEditable);

	//'Next' is the default progression
	item->setText(gotoColumn, "next");

	//Grab all actors in world and add their names to a combobox
	auto actorComboBox = new QComboBox(this);
	auto actorsInWorld = World::GetAllActorsInWorld();
	for (auto actor : actorsInWorld)
	{
		QString actorName = QString::fromStdString(actor->GetName());
		actorComboBox->addItem(actorName);
	}

	dialogueTree->setItemWidget(item, actorColumn, actorComboBox);

	//set condition combo box from FunctionSystem functions
	auto conditionComboBox = new QComboBox(this);
	conditionComboBox->addItem("");
	for (auto& functionName : functionSystem->GetFunctionNames())
	{
		QString conditionName = QString::fromStdString(functionName);
		conditionComboBox->addItem(conditionName);

		//Ref:https://forum.qt.io/topic/20998/qt5-new-signals-slots-syntax-does-not-work-solved/8
		connect(conditionComboBox, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
			this, &DialogueDock::SetConditionArgWidgets);
	}

	dialogueTree->setItemWidget(item, conditionColumn, conditionComboBox);
}

void DialogueDock::AddEmptyDialogueLine()
{
	auto selectedLine = dialogueTree->currentItem();
	auto item = new QTreeWidgetItem(dialogueTree, selectedLine);
	PopulateTreeItem(item);

	//refresh all the line values
	QTreeWidgetItemIterator it(dialogueTree);
	int lineIndex = 0;
	while (*it)
	{
		(*it)->setText(lineColumn, QString::number(lineIndex));
		lineIndex++;
		it++;
	}
}

void DialogueDock::DeleteLine()
{
	auto selectedLine = dialogueTree->currentItem();
	int index = dialogueTree->indexOfTopLevelItem(selectedLine);
	dialogueTree->takeTopLevelItem(index);
}

void DialogueDock::SaveDialogueToFile()
{
	QFileDialog saveDialog;
	QString saveName = saveDialog.getSaveFileName(this, "Save Dialogue", "Dialogues/");
	if (saveName.isEmpty()) return;

	std::wofstream os;
	os.open(saveName.toStdString(), std::ios_base::out);

	QTreeWidgetItemIterator it(dialogueTree);
	while (*it) 
	{
		auto lineText = (*it)->text(lineColumn);
		auto gotoText = (*it)->text(gotoColumn);

		//Have to do a bit more to get the text from the combobox
		auto actorComboBox = (QComboBox*)dialogueTree->itemWidget(*it, actorColumn);
		auto conditionComboBox = (QComboBox*)dialogueTree->itemWidget(*it, conditionColumn);

		auto conditionArgText = (*it)->text(conditionArgColumn);

		auto text = (*it)->text(textColumn);

		os << lineText.toStdWString() << "\n";
		os << gotoText.toStdWString() << "\n";
		os << actorComboBox->currentText().toStdWString() << "\n";
		os << conditionComboBox->currentText().toStdWString() << "\n";

		{
			auto widget = (QWidget*)dialogueTree->itemWidget(*it, conditionArgColumn);
			auto conditionString = conditionComboBox->currentText().toStdString();

			if (!conditionString.empty())
			{
				auto foundFunction = functionSystem->FindFunction(conditionString);

				auto args = foundFunction->GetArgTypes();
				int itemIndex = 0;
				for (auto& arg : args)
				{
					if (arg == typeid(std::string))
					{
						auto layout = (QVBoxLayout*)widget->layout();
						auto argWidget = (QLineEdit*)layout->itemAt(itemIndex)->widget();
						os << argWidget->text().toStdWString() << "\n";
					}

					itemIndex++;
				}
			}
		}

		os << conditionArgText.toStdWString() << "\n";
		os << text.toStdWString() << "\n";

		it++;
	}

	os.close();
}

void DialogueDock::LoadDialogueFile()
{
	QFileDialog loadDialog;
	QString loadName = loadDialog.getOpenFileName(this, "Open Dialogue", "Dialogues/");
	if (loadName.isEmpty()) return;

	//set dock title to filename (strip out the path with QFileInfo)
	this->setWindowTitle("Dialogue | " + QFileInfo(loadName).fileName());

	std::wifstream is;
	is.open(loadName.toStdString(), std::ios_base::in);

	dialogueTree->clear();


	while (!is.eof())
	{
		//Get all text
		std::wstring lineText;
		std::wstring gotoText;
		std::wstring actorText;
		std::wstring conditionText;
		std::vector<std::wstring> conditionArgText;
		std::wstring text;

		wchar_t line[1024];

		is.getline(line, 1024); 
		lineText.assign(line);
		if (lineText.empty())
		{
			break;
		}
		
		is.getline(line, 1024); 
		gotoText.assign(line);
		
		is.getline(line, 1024); 
		actorText.assign(line);

		is.getline(line, 1024);
		conditionText.assign(line);


		//Condition arg loading
		auto foundFunction = functionSystem->FindFunction(VString::wstos(conditionText));
		auto args = foundFunction->GetArgTypes();
		int itemIndex = 0;
		for (auto& arg : args)
		{
			if (arg == typeid(std::string))
			{
				is.getline(line, 1024);
				conditionArgText.emplace_back(line);
			}

			itemIndex++;
		}

		is.getline(line, 1024);
		text.assign(line);		
		

		//Populate widget items
		auto item = new QTreeWidgetItem(dialogueTree);
		PopulateTreeItem(item);

		item->setText(lineColumn, QString::fromStdWString(lineText));
		item->setText(gotoColumn, QString::fromStdWString(gotoText));
		item->setText(textColumn, QString::fromStdWString(text));

		{
			//Actor combobox
			//Find the matching existing entry in the combobox and set it per the index
			auto actorComboBox = (QComboBox*)dialogueTree->itemWidget(item, actorColumn);

			//findText() returns -1 if nothing is found and will place an empty entry in the combobox.
			//Have to be careful here on the findText() as well. QStrings work a bit funny with '\n' and '\r' I'm guessing.
			QString actorStr = QString::fromStdWString(actorText);
			int foundActorComboEntryIndex = actorComboBox->findText(actorStr);
			actorComboBox->setCurrentIndex(foundActorComboEntryIndex);
		}

		//Condition combobox
		{
			auto conditionComboBox = (QComboBox*)dialogueTree->itemWidget(item, conditionColumn);

			QString conditionStr = QString::fromStdWString(conditionText);
			int foundConditionComboEntryIndex = conditionComboBox->findText(conditionStr);
			conditionComboBox->setCurrentIndex(foundConditionComboEntryIndex);
		}

		//Condition Arg widgets
		{
			itemIndex = 0;
			for (auto& type : args)
			{
				if (type == typeid(std::string))
				{
					auto vLayout = new QVBoxLayout();
					vLayout->addWidget(new QLineEdit(foundFunction->GetName(itemIndex).c_str()));
					auto widg = new QWidget();
					widg->setLayout(vLayout);
					dialogueTree->setItemWidget(item, conditionArgColumn, widg);
				}
			}
		}
	}

	is.close();
}

void DialogueDock::SetConditionArgWidgets(const QString& conditionName)
{
	auto foundFunction = functionSystem->FindFunction(conditionName.toStdString());

	int nameIndex = 0;

	auto argTypes = foundFunction->GetArgTypes();
	for (auto& type : argTypes)
	{
		if (type == typeid(std::string))
		{
			auto vLayout = new QVBoxLayout();
			vLayout->addWidget(new QLineEdit(foundFunction->GetName(nameIndex).c_str()));
			auto widg = new QWidget();
			widg->setLayout(vLayout);
			dialogueTree->setItemWidget(dialogueTree->currentItem(), conditionArgColumn, widg);
		}

		nameIndex++;
	}
}
