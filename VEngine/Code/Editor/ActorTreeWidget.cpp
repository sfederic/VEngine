#include "vpch.h"
#include "ActorTreeWidget.h"
#include <qevent.h>
#include "Core/World.h"

ActorTreeWidget::ActorTreeWidget(QWidget* parent) : QTreeWidget(parent)
{
	//Ref:https://doc.qt.io/archives/qt-4.8/model-view-programming.html#using-drag-and-drop-with-item-views
	//Item flags need to be set as well for dragging/dropping to work
	setDragEnabled(true);
	viewport()->setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::InternalMove);
}

void ActorTreeWidget::dropEvent(QDropEvent* event)
{
	QModelIndex index = indexAt(event->pos());
	QTreeWidgetItem* item = itemFromIndex(index);
	if (item == nullptr) return;

	auto parentActorName = item->text(0).toStdString();
	auto parentActor = World::GetActorByName(parentActorName);

	//Assign dragged actor entry to the dropped actor
	if (dragChildActor != nullptr)
	{
		parentActor->AddChild(dragChildActor);
	}

	dragChildActor = nullptr;

	QTreeWidget::dropEvent(event);
}

void ActorTreeWidget::dragEnterEvent(QDragEnterEvent* event)
{
	//Grab the child actor currently being dragged
	QModelIndex index = indexAt(event->pos());
	QTreeWidgetItem* item = itemFromIndex(index);
	if (item == nullptr) return;

	auto childActorName = item->text(0).toStdString();
	dragChildActor = World::GetActorByName(childActorName);

	QTreeWidget::dragEnterEvent(event);
}
