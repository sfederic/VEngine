#include "WorldDock.h"
#include <qtreewidget.h>
#include <QTreeWidgetItem>
#include <qmenu.h>
#include "World.h"
#include <qboxlayout.h>
#include <qlineedit.h>
#include "Actors/IActorSystem.h"
#include "Actors/Actor.h"
#include "Camera.h"
#include "WorldEditor.h"
#include "Editor.h"
#include "Input.h"

WorldDock::WorldDock() : QDockWidget("World")
{
	//Search bar
	actorSearchBar = new QLineEdit(this);
	actorSearchBar->setPlaceholderText("Search Actors...");
	connect(actorSearchBar, &QLineEdit::textChanged, this, &WorldDock::SearchActors);

	//Actor Tree widget
	actorTreeWidget = new QTreeWidget(this);
	actorTreeWidget->setColumnCount(1);
	actorTreeWidget->setHeaderLabels(QStringList("Actors"));
	actorTreeWidget->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);

	actorTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(actorTreeWidget, &QTreeWidget::customContextMenuRequested, this, &WorldDock::ActorListContextMenu);

	connect(actorTreeWidget, &QTreeWidget::itemClicked, this, &WorldDock::ClickOnActorInList);
	connect(actorTreeWidget, &QTreeWidget::itemChanged, this, &WorldDock::ActorNameChanged);

	//Dock Layout
	auto vLayout = new QVBoxLayout(this);
	vLayout->addWidget(actorSearchBar);
	vLayout->addWidget(actorTreeWidget);

	auto worldWidget = new QWidget(this);
	worldWidget->setLayout(vLayout);

	setWidget(worldWidget);
}

void WorldDock::Tick()
{
	QAbstractItemView::SelectionMode selectionMode;

	if (Input::GetAsyncKey(Keys::ShiftLeft))
	{
		selectionMode = QAbstractItemView::SelectionMode::MultiSelection;
	}
	else
	{
		selectionMode = QAbstractItemView::SelectionMode::SingleSelection;
	}

	actorTreeWidget->setSelectionMode(selectionMode);
}

void WorldDock::PopulateWorldActorList()
{
    actorTreeWidget->clear();

	//Need to block signals because calling functions on tree items makes the connect()ed event fire
	actorTreeWidget->blockSignals(true);

	for (Actor* actor : world.GetAllActorsInWorld())
	{
		auto item = new QTreeWidgetItem(actorTreeWidget);
		item->setText(0, QString::fromStdString(actor->name));
		item->setFlags(item->flags() | Qt::ItemIsEditable);
	}

	actorTreeWidget->blockSignals(false);
}

void WorldDock::ClickOnActorInList(QTreeWidgetItem* item, int column)
{
	QString actorName = item->text(column);
	Actor* clickedActor = world.FindActorByName(actorName.toStdString());
	if (clickedActor)
	{
		worldEditor.pickedActor = clickedActor;
		worldEditor.pickedActors.insert(clickedActor);
		editor->SetActorProps(clickedActor);
	}
}

void WorldDock::ActorNameChanged(QTreeWidgetItem* item, int column)
{
	QString newActorName = item->text(column);

	Actor* actor = worldEditor.pickedActor;
	if (actor) //pickedActor should be set before this is hit in the other events
	{
		if (!actor->SetName(newActorName.toStdString()))
		{
			editor->Log(L"Could not change actor name. Name already exists.");
		}
	}
}

void WorldDock::ActorListContextMenu(const QPoint& pos)
{
	QPoint globalPos = actorTreeWidget->mapToGlobal(pos);

	QMenu actorListMenu;
	actorListMenu.addAction("Clear Selection", actorTreeWidget, &QTreeWidget::clearSelection);

	actorListMenu.exec(globalPos);
}

void WorldDock::SelectActorInList()
{
	actorTreeWidget->clearSelection();

	std::vector<std::string> actorNames;
	QList<QTreeWidgetItem*> listItems;

	for (auto actor : worldEditor.pickedActors)
	{
		std::string actorName = actor->name;
		auto foundItems = actorTreeWidget->findItems(QString::fromStdString(actorName), Qt::MatchExactly);

		//Names should be unique in list, even though findItems() returns a collection
		assert(foundItems.size() == 1);

		listItems.append(foundItems);
	}

	for (auto item : listItems)
	{
		actorTreeWidget->setItemSelected(item, true);
	}
}

//This is only working for single columns in the tree. If showing actor parenting through this list 
//later on, need to change this too in the item->text(int column) below.
void WorldDock::SearchActors()
{
	QString searchText = actorSearchBar->text().toLower();
	
	for (int i = 0; i < actorTreeWidget->topLevelItemCount(); i++)
	{
		QTreeWidgetItem* item = actorTreeWidget->topLevelItem(i);
		if (item->text(0).toLower().contains(searchText))
		{
			item->setHidden(false);
		}
		else
		{
			item->setHidden(true);
		}
	}
}