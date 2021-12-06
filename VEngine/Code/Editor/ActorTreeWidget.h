#pragma once
#include <qtreewidget.h>

struct ActorTreeWidget : public QTreeWidget
{
	ActorTreeWidget(QWidget* parent = nullptr) : QTreeWidget(parent) {}

	//disables key press searching in treewidgets by redefining as empty.
	virtual void keyboardSearch(const QString& search) {};
};
