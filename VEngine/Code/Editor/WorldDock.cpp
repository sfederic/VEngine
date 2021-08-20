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
	connect(actorTreeWidget, &QTreeWidget::itemDoubleClicked, this, &WorldDock::DoubleClickOnActorInList);

	setWidget(actorTreeWidget);
}

void WorldDock::PopulateWorldActorList()
{
    actorTreeWidget->clear();

	for (IActorSystem* actorSystem : world.activeActorSystems)
	{
		std::vector<Actor*> actors;
		actorSystem->GetActors(actors);

		for (Actor* actor : actors)
		{
			auto item = new QTreeWidgetItem(actorTreeWidget);
			item->setText(0, QString::fromStdString(actor->name));
			item->insertChild(0, new QTreeWidgetItem());
		}
	}
}

void WorldDock::ClickOnActorInList(QTreeWidgetItem* item, int column)
{
	QString string = item->text(0);
	Actor* clickedActor = world.FindActorByName(string.toStdString());
	if (clickedActor)
	{
		worldEditor.pickedActor = clickedActor;
		editor->ActorProps(clickedActor);
	}
}

void WorldDock::DoubleClickOnActorInList(QTreeWidgetItem* item, int column)
{
	QString string = item->text(0);
	Actor* clickedActor = world.FindActorByName(string.toStdString());
	if (clickedActor)
	{
		activeCamera->ZoomTo(clickedActor);
		worldEditor.pickedActor = clickedActor;
		editor->ActorProps(clickedActor);
	}
}
