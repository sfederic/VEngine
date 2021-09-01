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
	QListWidget* componentSystemList;

	QLabel* selectedActorSystemLabel;
	QLabel* selectedComponentSystemLabel;

	QLineEdit* actorSystemSearchBar;

	SystemDock();
	void PopulateSystemLists();
	void AddActorSystemsToWidget();
	void AddComponentSystemsToWidget();
	void ClickOnActorSystemItem(QListWidgetItem* item);
	void ClickOnComponentSystemItem(QListWidgetItem* item);
	void SearchActorSystems();
};
