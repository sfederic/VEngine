#include "WorldDock.h"
#include <qtreewidget.h>
#include <QTreeWidgetItem>
#include "World.h"
#include "Actors/IActorSystem.h"
#include "Actors/Actor.h"
#include "Camera.h"
#include "WorldEditor.h"
#include "Editor.h"

WorldDock::WorldDock() : QDockWidget("World")
{
	actorTreeWidget = new QTreeWidget();

	actorTreeWidget->setColumnCount(1);
	actorTreeWidget->setHeaderLabels(QStringList("Actors"));

	actorTreeWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	connect(actorTreeWidget, &QTreeWidget::itemClicked, this, &WorldDock::ClickOnActorInList);

	setWidget(actorTreeWidget);
}

void WorldDock::PopulateWorldActorList()
{
    actorTreeWidget->clear();

	//Need to block signals because calling functions on tree items makes the connect()ed event fire
	actorTreeWidget->blockSignals(true);

	for (IActorSystem* actorSystem : world.activeActorSystems)
	{
		std::vector<Actor*> actors;
		actorSystem->GetActors(actors);

		for (Actor* actor : actors)
		{
			auto item = new QTreeWidgetItem(actorTreeWidget);
			item->setText(0, QString::fromStdString(actor->name));
			item->setFlags(item->flags() | Qt::ItemIsEditable);
			item->insertChild(0, new QTreeWidgetItem());
		}
	}

	actorTreeWidget->blockSignals(false);

	connect(actorTreeWidget, &QTreeWidget::itemChanged, this, &WorldDock::ActorNameChanged);
}

void WorldDock::ClickOnActorInList(QTreeWidgetItem* item, int column)
{
	QString actorName = item->text(column);
	Actor* clickedActor = world.FindActorByName(actorName.toStdString());
	if (clickedActor)
	{
		worldEditor.pickedActor = clickedActor;
		editor->ActorProps(clickedActor);
	}
}

void WorldDock::ActorNameChanged(QTreeWidgetItem* item, int column)
{
	QString actorName = item->text(column);

	if (worldEditor.pickedActor) //pickedActor should be set before this is hit in the other events
	{
		worldEditor.pickedActor->name = actorName.toStdString();
	}
}
