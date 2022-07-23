#pragma once
#include <qtreewidget.h>

struct ActorTreeWidget : public QTreeWidget
{
	ActorTreeWidget(QWidget* parent = nullptr) : QTreeWidget(parent)
	{
		//Ref:https://doc.qt.io/archives/qt-4.8/model-view-programming.html#using-drag-and-drop-with-item-views
		//Item flags need to be set as well for dragging/dropping to work
		setDragEnabled(true);
		viewport()->setAcceptDrops(true);	
		setDropIndicatorShown(true);
		setDragDropMode(QAbstractItemView::InternalMove);
	}

	//disables key press searching in treewidgets by redefining as empty.
	virtual void keyboardSearch(const QString& search) {};
};
