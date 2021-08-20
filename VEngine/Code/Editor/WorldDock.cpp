#include "WorldDock.h"
#include <qtreewidget.h>
#include <QTreeWidgetItem>
#include "World.h"
#include "Actors/IActorSystem.h"
#include "Actors/Actor.h"

WorldDock::WorldDock() : QDockWidget("World")
{
	actorTreeWidget = new QTreeWidget();
	actorTreeWidget->setColumnCount(1);
	actorTreeWidget->setHeaderLabels(QStringList("Actors"));

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
