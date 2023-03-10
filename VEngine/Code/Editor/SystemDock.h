#pragma once

#include <qdockwidget.h>

class QListWidget;
class QListWidgetItem;
class QLabel;
class QLineEdit;

//Displays information for all Actor and Component systems
class SystemDock : public QDockWidget
{
public:
	SystemDock();
	void PopulateSystemLists();
	void AddActorSystemsToWidget();
	void ClickOnActorSystemItem(QListWidgetItem* item);
	void SearchActorSystems();

private:
	QListWidget* actorSystemList = nullptr;
	QLabel* selectedActorSystemLabel = nullptr;
	QLineEdit* actorSystemSearchBar = nullptr;
};
