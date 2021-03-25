#include "WorldDock.h"
#include <qlistview.h>
#include "World.h"
#include "WorldEditor.h"
#include "Camera.h"
#include <qlayout.h>
#include <qtreewidget.h>
#include <QListWidgetItem>
#include <qevent.h>
#include <qsortfilterproxymodel.h>
#include <QListWidgetItem>
#include "../EditorMainWindow.h"
#include "PropertiesDock.h"

QList<QTreeWidgetItem*> worldTreeItems;
QTreeWidget* worldTreeList;

WorldDock::WorldDock(const char* title) : QDockWidget(title)
{
    worldList = new QListWidget();
    worldList->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    connect(worldList, &QListWidget::itemClicked, this, &WorldDock::ClickOnListActor);

    //PopulateWorldList();

    worldSearch = new QLineEdit();
    worldSearch->setPlaceholderText("Search");
    connect(worldSearch, &QLineEdit::textChanged, this, &WorldDock::SearchWorldList);

    QVBoxLayout* worldVLayout = new QVBoxLayout();
    worldVLayout->addWidget(worldSearch);
    //worldVLayout->addWidget(worldList);

    //Tree widget for Actor Systems
    worldTreeList = new QTreeWidget();

    connect(worldTreeList, &QTreeWidget::itemClicked, this, &WorldDock::ClickOnListActorSystem);
    worldTreeList->setColumnCount(1);
    worldTreeList->setHeaderLabels(QStringList("Actor Systems"));

    worldVLayout->addWidget(worldList);
    worldVLayout->addWidget(worldTreeList);
    setLayout(worldVLayout);

    QWidget* worldWidget = new QWidget();
    worldWidget->setLayout(worldVLayout);
    setWidget(worldWidget);
}

void WorldDock::Tick()
{

}

void WorldDock::PopulateWorldList()
{
    World* world = GetWorld();

    for (int i = 0; i < world->actorSystems.size(); i++)
    {
        worldTreeItems.append(new QTreeWidgetItem(worldTreeList, QStringList(QString::fromStdWString(world->actorSystems[i]->name))));

        for (int actorIndex = 0; actorIndex < world->actorSystems[i]->actors.size(); actorIndex++)
        {
            QString actorName = QString::fromStdWString(world->actorSystems[i]->actors[actorIndex]->name);

            QTreeWidgetItem* child = new QTreeWidgetItem();
            child->setText(0, actorName);
            //NOTE: make sure Qt is cleaning up children with TreeView. Don't know if it's a leak
            worldTreeItems[i]->addChild(child);
        }
    }

    worldTreeList->insertTopLevelItems(0, worldTreeItems);

    //Old list without tree view
    worldList->clear();
    worldStringList.clear();

    for (int i = 0; i < world->actorSystems.size(); i++)
    {
        ActorSystem* ac = world->actorSystems[i];

        for (int j = 0; j < ac->actors.size(); j++)
        {
            QString name = QString::fromStdWString(ac->actors[j]->name);
            worldStringList.append(name);
        }
    }

    worldList->addItems(worldStringList);
}

void WorldDock::ClickOnListActorSystem(QTreeWidgetItem* listItem)
{
    QString string = listItem->text(0);
    Actor* clickedActor = GetWorld()->FindActorByString(string.toStdWString());
    if (clickedActor)
    {
        editorCamera.ZoomTo(clickedActor);
        gEditorMainWindow->propertiesDock->DisplayActorSystemProperties(clickedActor->GetActorSystem());
    }
}

void WorldDock::ClickOnListActor(QListWidgetItem* listItem)
{
    QString string = listItem->text();
    Actor* clickedActor = GetWorld()->FindActorByString(string.toStdWString());
    if (clickedActor)
    {
        editorCamera.ZoomTo(clickedActor);
        gEditorMainWindow->propertiesDock->DisplayActorSystemProperties(clickedActor->GetActorSystem());
    }
}

void WorldDock::SearchWorldList()
{
    QString searchText = "*" + worldSearch->text() + "*"; //Wildcards include items with text at any point

    //Old world list search
    worldList->clear();

    for (int i = 0; i < worldStringList.size(); i++)
    {
        if (worldStringList.indexOf(QRegExp(searchText, Qt::CaseInsensitive, QRegExp::Wildcard)) == 0)
        {
            worldList->addItem(worldStringList.at(i));
        }
    }
}