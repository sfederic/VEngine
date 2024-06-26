#pragma once

#include <qtreewidget.h>

class ActorTreeWidget : public QTreeWidget
{
public:
	ActorTreeWidget(QWidget* parent = nullptr) : QTreeWidget(parent) {}

	//Disables key press searching in tree widgets by redefining as empty.
	virtual void keyboardSearch(const QString&) {};
};
