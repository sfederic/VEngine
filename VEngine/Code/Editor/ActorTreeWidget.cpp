#include "vpch.h"
#include "ActorTreeWidget.h"
#include <qevent.h>

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

	QTreeWidget::dropEvent(event);
}
