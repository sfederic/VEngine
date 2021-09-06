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

	actorTreeWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	connect(actorTreeWidget, &QTreeWidget::itemClicked, this, &WorldDock::ClickOnActorInList);
	connect(actorTreeWidget, &QTreeWidget::itemChanged, this, &WorldDock::ActorNameChanged);

	setWidget(actorTreeWidget);
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
