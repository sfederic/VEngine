#include "DialogueDock.h"
#include <fstream>
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
	QString saveName = saveDialog.getSaveFileName(this, "Save Dialogue", "Dialogues/", ".dialog");

	std::ofstream os;
	os.open(saveName.toStdString(), std::ios_base::out);

	QTreeWidgetItemIterator it(dialogueTree);
	while (*it) 
	{
		auto lineText = (*it)->text(lineColumn);
		auto actorText = (*it)->text(actorColumn);
		auto intuitionText = (*it)->text(intuitionColumn);
		auto text = (*it)->text(textColumn);

		os << "Line\n" << lineText.toStdString() << "\n";
		os << "Actor\n" << actorText.toStdString() << "\n";
		os << "Intuition\n" << intuitionText.toStdString() << "\n";
		os << "Text\n" << text.toStdString() << "\n";

		it++;
	}

	os.close();
}

void DialogueDock::LoadDialogueFile()
{
	QFileDialog loadDialog;
	loadDialog.exec();
}
