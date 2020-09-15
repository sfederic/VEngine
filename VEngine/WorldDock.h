#pragma once

#include <qdockwidget.h>
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qstringlist.h>

class WorldDock : public QDockWidget
{
public: 
	WorldDock(const char* title);
	void Tick();

    void PopulateWorldList();
    void ClickOnListActorSystem(class QTreeWidgetItem* listItem);
    void ClickOnListActor(class QListWidgetItem* listItem);
    void SearchWorldList();

    QListWidget* worldList;
    QLineEdit* worldSearch;
    QStringList worldStringList;
};
