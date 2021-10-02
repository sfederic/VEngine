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

//columns for QTreeWidget
const int lineColumn = 0;
const int actorColumn = 1;
const int intuitionColumn = 2;
const int textColumn = 3;

DialogueDock::DialogueDock() : QDockWidget("Dialogue")
{
	//TREE WIDGET
	dialogueTree = new QTreeWidget(); 
	dialogueTree->setColumnCount(4);
	dialogueTree->setHeaderLabels({ "Line", "Actor", "Intuition", "Text"});


	//BUTTONS
	auto insertDialogueLineButton = new QPushButton("Insert Line");
	connect(insertDialogueLineButton, &QPushButton::clicked, this, &DialogueDock::AddEmptyDialogueLine);

	auto insertBranchDialogueButton = new QPushButton("Insert Branch");
	connect(insertBranchDialogueButton, &QPushButton::clicked, this, &DialogueDock::AddEmptyBranchDialogueLine);

	auto deleteLineButton = new QPushButton("Delete");
	connect(deleteLineButton, &QPushButton::clicked, this, &DialogueDock::DeleteLine);

	auto saveButton = new QPushButton("Save");
	saveButton->setFixedWidth(50);
	connect(saveButton, &QPushButton::clicked, this, &DialogueDock::SaveDialogueToFile);

	auto loadButton = new QPushButton("Load");
	loadButton->setFixedWidth(50);
	connect(loadButton, &QPushButton::clicked, this, &DialogueDock::LoadDialogueFile);

	//BUTTONS HORIZONTAL LAYOUT
	auto hLayout = new QHBoxLayout();
	hLayout->addWidget(insertDialogueLineButton);
	hLayout->addWidget(insertBranchDialogueButton);
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

	//Grab all actors in world and add their names to a combobox
	auto actorComboBox = new QComboBox(this);
	auto actorsInWorld = world.GetAllActorsInWorld();
	for (auto actor : actorsInWorld)
	{
		QString actorName = QString::fromStdString(actor->name);
		actorComboBox->addItem(actorName);
	}

	dialogueTree->setItemWidget(item, actorColumn, actorComboBox);
}

void DialogueDock::AddEmptyDialogueLine()
{
	auto selectedLine = dialogueTree->currentItem();
	auto item = new QTreeWidgetItem(dialogueTree, selectedLine);
	PopulateTreeItem(item);
}

void DialogueDock::AddEmptyBranchDialogueLine()
{
	auto selectedLine = dialogueTree->currentItem();
	auto item = new QTreeWidgetItem(selectedLine);
	PopulateTreeItem(item);
}

void DialogueDock::DeleteLine()
{
	auto selectedLine = dialogueTree->currentItem();
	int index = dialogueTree->indexOfTopLevelItem(selectedLine);
	dialogueTree->takeTopLevelItem(index);
}

//TODO: when you're saving dialogue, make sure the Intuition and Actor name fields correspond
//to something that actually exists in-game/world.
void DialogueDock::SaveDialogueToFile()
{
	QFileDialog saveDialog;
	QString saveName = saveDialog.getSaveFileName(this, "Save Dialogue", ".dialog");

	std::ofstream os;
	os.open(saveName.toStdString(), std::ios_base::out);

	QTreeWidgetItemIterator it(dialogueTree);
	while (*it) 
	{
		auto lineText = (*it)->text(lineColumn);

		//Have to do a bit more to get the text from the combobox
		QComboBox* actorComboBox = (QComboBox*)dialogueTree->itemWidget(*it, actorColumn);

		auto intuitionText = (*it)->text(intuitionColumn);
		auto text = (*it)->text(textColumn);

		os << lineText.toStdString() << "\n";
		os << actorComboBox->currentText().toStdString() << "\n";
		os << intuitionText.toStdString() << "\n";
		os << text.toStdString() << "\n";

		int numChildren = (*it)->childCount();
		os << numChildren << "\n";

		it++;
	}

	os.close();
}

void DialogueDock::LoadDialogueFile()
{
	QFileDialog loadDialog;
	QString loadName = loadDialog.getOpenFileName(this, "Open Dialogue");

	std::ifstream is;
	is.open(loadName.toStdString(), std::ios_base::in);

	dialogueTree->clear();

	//For figuring our parent items and linking children to them
	QTreeWidgetItem* previousItem = nullptr;

	while (!is.eof())
	{
		//Get all text
		std::string lineText;
		std::string actorText;
		std::string intuitionText;
		std::string text;
		std::string numChildren;

		char line[1024];

		is.getline(line, 1024); 
		lineText.assign(line);
		if (lineText.empty())
		{
			break;
		}
		
		is.getline(line, 1024); 
		actorText.assign(line);

		is.getline(line, 1024);
		intuitionText.assign(line);

		is.getline(line, 1024);
		text.assign(line);		
		
		is.getline(line, 1024);
		numChildren.assign(line);

		//populate widget items
		QTreeWidgetItem* item = nullptr;

		//attach to TreeWidgetItem if branching dialogue
		int childCount = std::stoi(numChildren);
		if (childCount == 0)
		{
			item = new QTreeWidgetItem(dialogueTree);
		}
		else if (childCount > 0)
		{
			item = new QTreeWidgetItem(dialogueTree, previousItem);
		}

		previousItem = item;

		PopulateTreeItem(item);

		item->setText(lineColumn, QString::fromStdString(lineText));
		item->setText(intuitionColumn, QString::fromStdString(intuitionText));
		item->setText(textColumn, QString::fromStdString(text));

		//Find the matching existing entry in the combobox and set it per the index
		QComboBox* actorComboBox = (QComboBox*)dialogueTree->itemWidget(item, actorColumn);

		//findText() returns -1 if nothing is found and will place an empty entry in the combobox.
		//Have to be careful here on the findText() as well. QStrings work a bit funny with '\n' and '\r' I'm guessing.
		QString actorStr = QString::fromStdString(actorText);
		int foundComboEntryIndex = actorComboBox->findText(actorStr);
		actorComboBox->setCurrentIndex(foundComboEntryIndex);
	}

	is.close();
}
