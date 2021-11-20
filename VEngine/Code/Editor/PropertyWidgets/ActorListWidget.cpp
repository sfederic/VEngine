#include "ActorListWidget.h"
#include "Actors/Actor.h"
#include <qlistwidget.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include "World.h"
#include "Editor/Editor.h"

ActorListWidget::ActorListWidget(Property value_)
{
	prop = value_;

	value = (std::vector<Actor*>*)value_.data;

	actorNameListWidget = new QListWidget(this);
	actorNameListWidget->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
	connect(actorNameListWidget, &QListWidget::itemChanged, this, &ActorListWidget::SetValue);
	ResetValue();

	addButton = new QPushButton("+", this);
	connect(addButton, &QPushButton::clicked, this, &ActorListWidget::AddItem);
	deleteButton = new QPushButton("-", this);
	connect(deleteButton, &QPushButton::clicked, this, &ActorListWidget::DeleteItems);

	auto vLayout = new QVBoxLayout(this);
	vLayout->addWidget(actorNameListWidget);
	vLayout->addWidget(addButton);
	vLayout->addWidget(deleteButton);

	setLayout(vLayout);
}

void ActorListWidget::SetValue(QListWidgetItem* item)
{
	if (actorNameListWidget->count() == 0) return;

	std::string actorName = item->text().toStdString();
	Actor* foundActor = world.GetActor(actorName);
	if (foundActor)
	{
		for (auto actor : *value)
		{
			if (actor == foundActor)
			{
				editor->Log("Actor already in list widget");
				actorNameListWidget->removeItemWidget(item);
				return;
			}
		}

		value->push_back(foundActor);
	}
}

void ActorListWidget::ResetValue()
{
	actorNameListWidget->clear();

	for (auto actor : *value)
	{
		QString actorName = QString::fromStdString(actor->name);
		auto item = new QListWidgetItem(actorName, actorNameListWidget);
		item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
	}
}

void ActorListWidget::AddItem()
{
	auto item = new QListWidgetItem("empty", actorNameListWidget);
	item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
}

void ActorListWidget::DeleteItems()
{
	auto items = actorNameListWidget->selectedItems();
	for (auto item : items)
	{
		for (int i = 0; i < value->size(); i++)
		{
			if (value->at(i)->name == item->text().toStdString())
			{
				value->erase(value->begin() + i);
			}
		}
	}

	qDeleteAll(items);
}
