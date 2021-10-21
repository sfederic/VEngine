#include "ActorListWidget.h"
#include "Actors/Actor.h"
#include <qlistwidget.h>
#include <qpushbutton.h>
#include <qlayout.h>

ActorListWidget::ActorListWidget(Property value_)
{
	prop = value_;

	value = (std::vector<Actor*>*)value_.data;

	actorNamelistWidget = new QListWidget(this);
	actorNamelistWidget->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
	SetValue();

	addButton = new QPushButton("+", this);
	connect(addButton, &QPushButton::clicked, this, &ActorListWidget::AddItem);
	deleteButton = new QPushButton("-", this);
	connect(deleteButton, &QPushButton::clicked, this, &ActorListWidget::DeleteItems);

	auto vLayout = new QVBoxLayout(this);
	vLayout->addWidget(actorNamelistWidget);
	vLayout->addWidget(addButton);
	vLayout->addWidget(deleteButton);

	setLayout(vLayout);
}

void ActorListWidget::SetValue()
{
	for (auto actor : *value)
	{
		QString actorName = QString::fromStdString(actor->name);
		auto item = new QListWidgetItem(actorName, actorNamelistWidget);
		item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
	}
}

void ActorListWidget::ResetValue()
{
	
}

void ActorListWidget::AddItem()
{
	auto item = new QListWidgetItem("empty", actorNamelistWidget);
	item->setFlags(item->flags() | Qt::ItemFlag::ItemIsEditable);
}

void ActorListWidget::DeleteItems()
{
	auto items = actorNamelistWidget->selectedItems();
	qDeleteAll(items);
}
