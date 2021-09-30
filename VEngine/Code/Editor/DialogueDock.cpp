#include "DialogueDock.h"
#include <qtreewidget.h>
#include <qstandarditemmodel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qfiledialog.h>
#include <qcombobox.h>
#include "World.h"
#include "Actors/Actor.h"

//columns for QTreeWidget
const int lineColumn = 0;
const int textColumn = 1;
const int actorColumn = 2;
const int intuitionColumn = 3;

DialogueDock::DialogueDock() : QDockWidget("Dialogue")
{
	//TREE WIDGET
	dialogueTree = new QTreeWidget(); 
	dialogueTree->setColumnCount(4);
	dialogueTree->setHeaderLabels({ "Line", "Text", "Actor", "Intuition" });
	dialogueTree->setColumnWidth(0, 20);
	dialogueTree->setColumnWidth(1, 750);


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
	saveDialog.getSaveFileName(this, "Save Dialogue", ".dialog");
}

void DialogueDock::LoadDialogueFile()
{
	QFileDialog loadDialog;
	loadDialog.exec();
}
