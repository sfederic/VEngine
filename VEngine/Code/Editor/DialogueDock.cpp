#include "DialogueDock.h"
#include <fstream>
#include <qtreewidget.h>
#include <qstandarditemmodel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qfiledialog.h>
#include <qcombobox.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include "World.h"
#include "Actors/Actor.h"
#include "Gameplay/ConditionSystem.h"

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
	auto actorsInWorld = world.GetAllActorsInWorld();
	for (auto actor : actorsInWorld)
	{
		QString actorName = QString::fromStdString(actor->name);
		actorComboBox->addItem(actorName);
	}

	dialogueTree->setItemWidget(item, actorColumn, actorComboBox);

	//set condition combo box from ConditionSystem functions
	auto conditionComboBox = new QComboBox(this);
	conditionComboBox->addItem("");
	for (auto& condition : conditionSystem.conditions)
	{
		QString conditionName = QString::fromStdString(condition.first);
		conditionComboBox->addItem(conditionName);
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

	std::ofstream os;
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

		os << lineText.toStdString() << "\n";
		os << gotoText.toStdString() << "\n";
		os << actorComboBox->currentText().toStdString() << "\n";
		os << conditionComboBox->currentText().toStdString() << "\n";
		os << conditionArgText.toStdString() << "\n";
		os << text.toStdString() << "\n";

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

	std::ifstream is;
	is.open(loadName.toStdString(), std::ios_base::in);

	dialogueTree->clear();


	while (!is.eof())
	{
		//Get all text
		std::string lineText;
		std::string gotoText;
		std::string actorText;
		std::string conditionText;
		std::string conditionArgText;
		std::string text;

		char line[1024];

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

		is.getline(line, 1024);
		conditionArgText.assign(line);

		is.getline(line, 1024);
		text.assign(line);		
		
		//Populate widget items
		auto item = new QTreeWidgetItem(dialogueTree);
		PopulateTreeItem(item);

		item->setText(lineColumn, QString::fromStdString(lineText));
		item->setText(gotoColumn, QString::fromStdString(gotoText));
		item->setText(conditionArgColumn, QString::fromStdString(conditionArgText));
		item->setText(textColumn, QString::fromStdString(text));

		{
			//Actor combobox
			//Find the matching existing entry in the combobox and set it per the index
			auto actorComboBox = (QComboBox*)dialogueTree->itemWidget(item, actorColumn);

			//findText() returns -1 if nothing is found and will place an empty entry in the combobox.
			//Have to be careful here on the findText() as well. QStrings work a bit funny with '\n' and '\r' I'm guessing.
			QString actorStr = QString::fromStdString(actorText);
			int foundActorComboEntryIndex = actorComboBox->findText(actorStr);
			actorComboBox->setCurrentIndex(foundActorComboEntryIndex);
		}

		//Condition combobox
		{
			auto conditionComboBox = (QComboBox*)dialogueTree->itemWidget(item, conditionColumn);

			QString conditionStr = QString::fromStdString(conditionText);
			int foundConditionComboEntryIndex = conditionComboBox->findText(conditionStr);
			conditionComboBox->setCurrentIndex(foundConditionComboEntryIndex);
		}
	}

	is.close();
}
