#pragma once
#include <qtreewidget.h>

class QDropEvent;

struct ActorTreeWidget : public QTreeWidget
{
	ActorTreeWidget(QWidget* parent = nullptr);
	
	//Disables key press searching in treewidgets by redefining as empty.
	virtual void keyboardSearch(const QString& search) {};

protected:
	//Event called when actors in world list are dragged and dropped on each other in list.
	virtual void dropEvent(QDropEvent* event) override;
};
