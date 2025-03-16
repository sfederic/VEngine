
#include "WorldDock.h"
#include <QTreeWidgetItem>
#include <qmenu.h>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include "ActorTreeWidget.h"
#include "Actors/IActorSystem.h"
#include "Actors/Actor.h"
#include "Actors/ActorSystemCache.h"
#include "Core/World.h"
#include "Core/WorldEditor.h"
#include "Core/Log.h"
#include "Editor.h"
#include "Core/Input.h"

WorldDock::WorldDock() : QDockWidget("World")
{
	//Search bar
	actorSearchBar = new QLineEdit(this);
	actorSearchBar->setPlaceholderText("Search Actors...");
	connect(actorSearchBar, &QLineEdit::textChanged, this, &WorldDock::SearchActors);

	//System filter combobox
	actorTypeComboBox = new QComboBox(this);
	actorTypeComboBox->addItem("All");
	for (std::string& actorSystemName : ActorSystemCache::Get().GetAllActorSystemNames())
	{
		actorTypeComboBox->addItem(QString::fromStdString(actorSystemName));
	}
	connect(actorTypeComboBox, SIGNAL(QComboBox::currentIndexChanged(int)),
		this, SLOT(WorldDock::ActorTypeFilterChanged));

	//Actor Tree widget
	actorTreeWidget = new ActorTreeWidget(this);
	actorTreeWidget->setColumnCount(1);
	actorTreeWidget->setHeaderLabels(QStringList("Actors"));
	actorTreeWidget->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
	//turn on sorting (click on column header to sort)
	actorTreeWidget->setSortingEnabled(true);
	actorTreeWidget->sortByColumn(0, Qt::SortOrder::AscendingOrder);

	actorTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(actorTreeWidget, &QTreeWidget::customContextMenuRequested, this, &WorldDock::ActorListContextMenu);

	connect(actorTreeWidget, &QTreeWidget::itemClicked, this, &WorldDock::ClickOnActorInList);
	connect(actorTreeWidget, &QTreeWidget::itemSelectionChanged, this, &WorldDock::ArrowSelectActorInList);
	connect(actorTreeWidget, &QTreeWidget::itemChanged, this, &WorldDock::ActorNameChanged);

	//Dock Layout
	auto vLayout = new QVBoxLayout(this);
	vLayout->addWidget(actorSearchBar);
	vLayout->addWidget(actorTypeComboBox);
	vLayout->addWidget(actorTreeWidget);

	auto worldWidget = new QWidget(this);
	worldWidget->setLayout(vLayout);

	setWidget(worldWidget);
}

void WorldDock::Tick()
{
	if (Input::GetKeyHeld(Keys::Ctrl))
	{
		actorListSelectionMode = QAbstractItemView::SelectionMode::MultiSelection;
	}
	else
	{
		actorListSelectionMode = QAbstractItemView::SelectionMode::SingleSelection;
	}

	actorTreeWidget->setSelectionMode(actorListSelectionMode);
}

void WorldDock::PopulateWorldActorList()
{
	actorTreeWidget->clear();

	//Need to block signals because calling functions on tree items makes the connect()ed event fire
	actorTreeWidget->blockSignals(true);

	//clear()s are here because these maps are added to in ActorSystem::Add() calls
	//but there's no way to refresh them before Deserialising data.
	World::Get().ClearAllActorsFromWorld();

	auto allActorsInWorld = World::Get().GetAllActorsInWorld();
	for (auto actor : allActorsInWorld)
	{
		auto item = new QTreeWidgetItem(actorTreeWidget);
		item->setText(0, QString::fromStdString(actor->GetName()));
		item->setFlags(item->flags() | Qt::ItemIsEditable);

		World::Get().AddActorToWorld(actor);
	}

	actorTreeWidget->blockSignals(false);
}

void WorldDock::ClickOnActorInList(QTreeWidgetItem* item, int column)
{
	QString actorName = item->text(column);
	Actor* clickedActor = World::Get().GetActorByName(actorName.toStdString());
	if (clickedActor)
	{
		WorldEditor::SetPickedActor(clickedActor);

		if (actorListSelectionMode == QAbstractItemView::SelectionMode::MultiSelection)
		{
			WorldEditor::AddPickedActor(clickedActor);
		}
		else
		{
			WorldEditor::ClearPickedActors();
		}

		Editor::Get().SetActorProps(clickedActor);
	}
}

void WorldDock::ArrowSelectActorInList()
{
	auto items = actorTreeWidget->selectedItems();
	if (!items.empty())
	{
		WorldEditor::ClearPickedActors();

		QString pickedActorName = items[0]->text(0);
		auto pickedActor = World::Get().GetActorByName(pickedActorName.toStdString());

		WorldEditor::SetPickedActor(pickedActor);

		Editor::Get().SetActorProps(pickedActor);
	}
}

void WorldDock::ActorNameChanged(QTreeWidgetItem* item, int column)
{
	QString newActorName = item->text(column);

	Actor* actor = WorldEditor::GetPickedActor();
	assert(actor); //pickedActor should be set before this is hit in the other events

	if (!actor->SetName(newActorName.toStdString()))
	{
		Log("Could not change actor name from %s to %s. Name already exists.",
			actor->GetName().c_str(), newActorName.toStdString().c_str());

		//Reset item text
		item->setText(column, QString::fromStdString(actor->GetName()));
	}
}

void WorldDock::ActorListContextMenu(const QPoint& pos)
{
	QPoint globalPos = actorTreeWidget->mapToGlobal(pos);

	QMenu actorListMenu;
	actorListMenu.addAction("Clear Selection", actorTreeWidget, &QTreeWidget::clearSelection);

	actorListMenu.exec(globalPos);
}

void WorldDock::ActorTypeFilterChanged(int index)
{
	const QString itemText = actorTypeComboBox->itemText(index);

	//Set all actors
	if (itemText == "All")
	{
		PopulateWorldActorList();
		return;
	}

	//Populate world actor list based on actor type

	actorTreeWidget->clear();
	actorTreeWidget->blockSignals(true);

	IActorSystem* actorSystem = ActorSystemCache::Get().GetSystem(itemText.toStdString());

	for (Actor* actor : actorSystem->GetActorsAsBaseClass())
	{
		auto item = new QTreeWidgetItem(actorTreeWidget);
		item->setText(0, QString::fromStdString(actor->GetName()));
		item->setFlags(item->flags() | Qt::ItemIsEditable);
	}

	actorTreeWidget->blockSignals(false);
}

void WorldDock::SelectActorInList()
{
	actorTreeWidget->blockSignals(true);

	actorTreeWidget->clearSelection();

	std::vector<std::string> actorNames;
	QList<QTreeWidgetItem*> listItems;

	for (auto actor : WorldEditor::GetPickedActors())
	{
		std::string actorName = actor->GetName();

		//The Qt::MatchRecursive flag is what moves the find through the entire actor tree
		auto foundItems = actorTreeWidget->findItems(
			QString::fromStdString(actorName), Qt::MatchExactly | Qt::MatchRecursive);

		listItems.append(foundItems);
	}

	for (auto item : listItems)
	{
		item->setSelected(true);
	}

	actorTreeWidget->blockSignals(false);
}

void WorldDock::AddActorToList(Actor* actor)
{
	actorTreeWidget->blockSignals(true);

	auto item = new QTreeWidgetItem(actorTreeWidget);

	item->setText(0, QString::fromStdString(actor->GetName()));
	item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

	actorTreeWidget->blockSignals(false);
}

void WorldDock::RemoveActorFromList()
{
	actorTreeWidget->blockSignals(true);

	QList<QTreeWidgetItem*> foundItems;
	for (Actor* actor : WorldEditor::GetPickedActors())
	{
		std::string actorName = actor->GetName();
		foundItems = actorTreeWidget->findItems(QString::fromStdString(actorName), Qt::MatchExactly);
		assert(foundItems.size() == 1);
	}

	for (auto item : foundItems)
	{
		actorTreeWidget->removeItemWidget(item, 0);
	}

	actorTreeWidget->blockSignals(false);
}

//This is only working for single columns in the tree. If showing actor parenting through this list 
//later on, need to change this too in the item->text(int column) below.
void WorldDock::SearchActors()
{
	const QString searchText = actorSearchBar->text().toLower();

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