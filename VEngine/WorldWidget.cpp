#include "WorldWidget.h"
#include "World.h"
#include "Actor.h"
#include "Camera.h"
#include "WorldEditor.h"
#include <qboxlayout.h>

void WorldWidget::Tick()
{
    worldList->clear();

    World* world = GetWorld();
    for (int i = 0; i < world->actorSystems.size(); i++)
    {
        ActorSystem* ac = world->actorSystems[i];

        for (int j = 0; j < ac->actors.size(); j++)
        {
            QString name = QString::fromStdWString(ac->name) + QString::number(j);
            worldList->addItem(name);
        }
    }
}

void WorldWidget::ClickOnListActor(QListWidgetItem* listItem)
{
    Actor* clickedActor = gWorldEditor.pickedActor;
    if (clickedActor)
    {
        editorCamera.ZoomTo(clickedActor);
    }
}

WorldWidget::WorldWidget(QWidget* parent) : QWidget(parent)
{
    worldList = new QListWidget();
    connect(worldList, SIGNAL(&QListWidget::itemClicked(QListWidgetItem)), this, SLOT(WorldWidget::ClickOnListActor(QListWidgetItem*)));

    //Populate list first time
    World* world = GetWorld();
    for (int i = 0; i < world->actorSystems.size(); i++)
    {
        ActorSystem* ac = world->actorSystems[i];

        for (int j = 0; j < ac->actors.size(); j++)
        {
            QString name = QString::fromStdWString(ac->name) + QString::number(j);
            worldList->addItem(name);
        }
    }

    worldSearch = new QLineEdit();
    worldSearch->setPlaceholderText("Search");
    connect(worldSearch, &QLineEdit::textChanged, this, &WorldWidget::searchWorldList);

    QVBoxLayout* worldVLayout = new QVBoxLayout();
    worldVLayout->addWidget(worldSearch);
    worldVLayout->addWidget(worldList);

    setLayout(worldVLayout);
}

void WorldWidget::searchWorldList()
{
    QString searchText = "*" + worldSearch->text() + "*"; //Wildcards include items with text at any point
    //worldStringList = directory.entryList(QStringList(searchText), QDir::Files);

    worldList->clear();
    worldList->addItems(worldStringList);
}