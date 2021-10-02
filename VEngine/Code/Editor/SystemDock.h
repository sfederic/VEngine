#pragma once
#include <qdockwidget.h>

class QListWidget;
class QListWidgetItem;
class QLabel;
class QLineEdit;

//Displays information for all Actor and Component systems
struct SystemDock : public QDockWidget
{
	QListWidget* actorSystemList;
	QLabel* selectedActorSystemLabel;
	QLineEdit* actorSystemSearchBar;

	SystemDock();
	void PopulateSystemLists();
	void AddActorSystemsToWidget();
	void ClickOnActorSystemItem(QListWidgetItem* item);
	void SearchActorSystems();
};
