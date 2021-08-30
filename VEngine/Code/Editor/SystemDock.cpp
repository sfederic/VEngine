#include "SystemDock.h"
#include <qtreewidget.h>
#include <qboxlayout.h>
#include "World.h"
#include "Actors/IActorSystem.h"
#include "Actors/Actor.h"
#include "Components/IComponentSystem.h"
#include "Components/Component.h"

SystemDock::SystemDock() : QDockWidget("Systems")
{
	//Setup actorsystem treewidget
	actorSystemTreeWidget = new QTreeWidget(this);
	actorSystemTreeWidget->setColumnCount(1);
	actorSystemTreeWidget->setHeaderLabels(QStringList("Actor Systems"));
	actorSystemTreeWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	connect(actorSystemTreeWidget, &QTreeWidget::itemClicked, this, &SystemDock::ClickOnActorSystem);

	//setup componentsystem treewidget
	componentSystemTreeWidget = new QTreeWidget(this);
	componentSystemTreeWidget->setColumnCount(1);
	componentSystemTreeWidget->setHeaderLabels(QStringList("Component Systems"));
	componentSystemTreeWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	connect(componentSystemTreeWidget, &QTreeWidget::itemClicked, this, &SystemDock::ClickOnComponentSystem);

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->addWidget(actorSystemTreeWidget);
	vLayout->addWidget(componentSystemTreeWidget);

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
	actorSystemTreeWidget->clear();

	for (IActorSystem* actorSystem : world.activeActorSystems)
	{
		auto item = new QTreeWidgetItem(actorSystemTreeWidget);
		item->setText(0, QString::fromStdString(actorSystem->name));

		std::vector<Actor*> actors;
		actorSystem->GetActors(actors);
		for (Actor* actor : actors)
		{
			auto childItem = new QTreeWidgetItem();
			childItem->setText(0, QString::fromStdString(actor->name));
			item->addChild(childItem);
		}
	}
}

void SystemDock::AddComponentSystemsToWidget()
{
	componentSystemTreeWidget->clear();

	for (IComponentSystem* componentSystem : world.activeComponentSystems)
	{
		auto item = new QTreeWidgetItem(componentSystemTreeWidget);
		item->setText(0, QString::fromStdString(componentSystem->name));

		std::vector<Component*> components;
		componentSystem->GetComponents(components);
		for (Component* component : components)
		{
			auto childItem = new QTreeWidgetItem();

			//Set component name in UI as system name + component index
			std::string componentName = componentSystem->name;
			componentName += std::to_string(component->index);
			childItem->setText(0, QString::fromStdString(componentName));

			item->addChild(childItem);
		}
	}
}

void SystemDock::ClickOnActorSystem()
{
}

void SystemDock::ClickOnComponentSystem()
{
}
