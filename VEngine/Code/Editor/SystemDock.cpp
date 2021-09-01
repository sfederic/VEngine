#include "SystemDock.h"
#include <qlistwidget.h>
#include <qboxlayout.h>
#include "World.h"
#include "Actors/IActorSystem.h"
#include "Actors/Actor.h"
#include "Actors/ActorSystemCache.h"
#include "Components/IComponentSystem.h"
#include "Components/Component.h"
#include "WorldEditor.h"
#include "Editor.h"
#include <qlabel.h>
#include <qlineedit.h>

SystemDock::SystemDock() : QDockWidget("Systems")
{
	//Setup actorsystem list widget
	actorSystemList = new QListWidget(this);
	actorSystemList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	connect(actorSystemList, &QListWidget::itemClicked, this, &SystemDock::ClickOnActorSystemItem);

	selectedActorSystemLabel = new QLabel("Selected Actor System");

	//setup actorsystem searchbar 
	actorSystemSearchBar = new QLineEdit();
	actorSystemSearchBar->setPlaceholderText("Search...");
	connect(actorSystemSearchBar, &QLineEdit::textChanged, this, &SystemDock::SearchActorSystems);

	//setup componentsystem list widget
	componentSystemList = new QListWidget(this);
	componentSystemList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	connect(componentSystemList, &QListWidget::itemClicked, this, &SystemDock::ClickOnComponentSystemItem);

	selectedComponentSystemLabel = new QLabel("Selected Component System");

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->addWidget(selectedActorSystemLabel);
	vLayout->addWidget(actorSystemSearchBar);
	vLayout->addWidget(actorSystemList);
	vLayout->addWidget(selectedComponentSystemLabel);
	vLayout->addWidget(componentSystemList);

	auto systemWidget = new QWidget(this);
	systemWidget->setLayout(vLayout);

	setWidget(systemWidget);
}

void SystemDock::PopulateSystemLists()
{
	AddActorSystemsToWidget();
	AddComponentSystemsToWidget();
}

void SystemDock::AddActorSystemsToWidget()
{
	actorSystemList->clear();

	for (IActorSystem* actorSystem : world.activeActorSystems)
	{
		auto item = new QListWidgetItem(actorSystemList);
		item->setText(QString::fromStdString(actorSystem->name));
	}
}

void SystemDock::AddComponentSystemsToWidget()
{
	componentSystemList->clear();

	for (IComponentSystem* componentSystem : world.activeComponentSystems)
	{
		auto item = new QListWidgetItem(componentSystemList);
		item->setText(QString::fromStdString(componentSystem->name));
	}
}

void SystemDock::ClickOnActorSystemItem(QListWidgetItem* item)
{
	QString actorSystemName = item->text();

	//Set spawn system based on the actorsystem clicked in the dock
	worldEditor.spawnSystem = actorSystemCache.Get(actorSystemName.toStdString());

	selectedActorSystemLabel->setText(actorSystemName);
}

void SystemDock::ClickOnComponentSystemItem(QListWidgetItem* item)
{
	//TODO: figure out if there is some sort of spawning on a component basis thing to do later

	QString componentSystemName = item->text();

	selectedComponentSystemLabel->setText(componentSystemName);
}

void SystemDock::SearchActorSystems()
{
	QString searchText = actorSystemSearchBar->text().toLower();

	for (int i = 0; i < actorSystemList->count(); i++)
	{
		QListWidgetItem* item = actorSystemList->item(i);
		if (item->text().toLower().contains(searchText))
		{
			item->setHidden(false);
		}
		else
		{
			item->setHidden(true);
		}
	}
}
