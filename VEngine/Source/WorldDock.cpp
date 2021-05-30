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
#include "ActorSystemFactory.h"
#include <qpushbutton.h>
#include <qdialog.h>
#include <qlabel.h>

QList<QTreeWidgetItem*> worldTreeItems;
QTreeWidget* worldTreeList;

WorldDock::WorldDock(const char* title) : QDockWidget(title)
{
    //Add/Create new actorsystem button
    auto addActorSystemButton = new QPushButton("Add Actor System");
    connect(addActorSystemButton, &QPushButton::clicked, this, &WorldDock::CreateNewActorSystem);

    //Search bar
    worldSearch = new QLineEdit();
    worldSearch->setPlaceholderText("Search");
    connect(worldSearch, &QLineEdit::textChanged, this, &WorldDock::SearchWorldList);

    //Actor systems list to choose actor spawning 
    actorSystemList = new QListWidget();
    actorSystemList->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    connect(actorSystemList, &QListWidget::itemClicked, this, &WorldDock::ClickOnActorSystem);

    PopulateActorSystemList();

    QVBoxLayout* worldVLayout = new QVBoxLayout();
    worldVLayout->addWidget(addActorSystemButton);
    worldVLayout->addWidget(worldSearch);

    //currently active actor systems in world list
    worldTreeList = new QTreeWidget();

    connect(worldTreeList, &QTreeWidget::itemClicked, this, &WorldDock::ClickOnActor);
    worldTreeList->setColumnCount(1);
    worldTreeList->setHeaderLabels(QStringList("Active Actor Systems"));

    worldVLayout->addWidget(actorSystemList);
    worldVLayout->addWidget(worldTreeList);
    setLayout(worldVLayout);

    QWidget* worldWidget = new QWidget();
    worldWidget->setLayout(worldVLayout);
    setWidget(worldWidget);
}

void WorldDock::Tick()
{

}

void WorldDock::PopulateActorSystemList()
{
    actorSystemList->clear();
    worldStringList.clear();

    auto& actorSystems = GetWorld()->actorSystems;

    for(int i = 0; i < actorSystems.size(); i++)
    {
        QString name = QString::fromStdString(actorSystems[i]->name);
        worldStringList.append(name);
    }

    actorSystemList->addItems(worldStringList);
}

void WorldDock::PopulateWorldList()
{
    worldTreeItems.clear();
    worldTreeList->clear();

    World* world = GetWorld();

    for (int i = 0; i < world->actorSystems.size(); i++)
    {
        worldTreeItems.append(new QTreeWidgetItem(worldTreeList, QStringList(QString::fromStdString(world->actorSystems[i]->name))));

        for (int actorIndex = 0; actorIndex < world->actorSystems[i]->actors.size(); actorIndex++)
        {
            QString actorName = QString::fromStdString(world->actorSystems[i]->actors[actorIndex]->name);

            QTreeWidgetItem* child = new QTreeWidgetItem();
            child->setText(0, actorName);
            //NOTE: make sure Qt is cleaning up children with TreeView. Don't know if it's a leak
            worldTreeItems[i]->addChild(child);
        }
    }

    worldTreeList->insertTopLevelItems(0, worldTreeItems);
}

void WorldDock::ClickOnActor(QTreeWidgetItem* listItem)
{
    QString string = listItem->text(0);
    Actor* clickedActor = GetWorld()->FindActorByString(string.toStdString());
    if (clickedActor)
    {
        editorCamera.ZoomTo(clickedActor);
        gWorldEditor.pickedActor = clickedActor;
        gEditorSystem->DisplayActorSystemProperties(clickedActor);
    }
}

void WorldDock::ClickOnActorSystem(QListWidgetItem* listItem)
{
    QString actorSystemName = listItem->text();
    ActorSystem* actorSystem = ActorSystemFactory::GetActorSystem(actorSystemName.toStdString().c_str());
    ActorSystemFactory::SetCurrentActiveActorSystem(actorSystem);
}

void WorldDock::SearchWorldList()
{
    QString searchText = "*" + worldSearch->text() + "*"; //Wildcards include items with text at any point

    for (int i = 0; i < worldStringList.size(); i++)
    {
        if (worldStringList.indexOf(QRegExp(searchText, Qt::CaseInsensitive, QRegExp::Wildcard)) == 0)
        {
            actorSystemList->addItem(worldStringList.at(i));
        }
    }
}

void WorldDock::CreateNewActorSystem()
{
    QDialog actorSystemPopup;
    actorSystemPopup.setWindowTitle("Add New Actor System");
    
    QGridLayout* grid = new QGridLayout();

    QLineEdit name;
    QLineEdit shader;
    QLineEdit model;
    QLineEdit texture;

    //TODO: would be nice to have custom dropdowns that list all current models/shaders in the system,
    //but who's going to do that? That's a lot of work

    grid->addWidget(new QLabel("Name:"), 0, 0);
    grid->addWidget(&name, 0, 1);

    grid->addWidget(new QLabel("Shader:"), 1, 0);
    grid->addWidget(&shader, 1, 1);

    grid->addWidget(new QLabel("Model:"), 2, 0);
    grid->addWidget(&model, 2, 1);

    grid->addWidget(new QLabel("Texture:"), 3, 0);
    grid->addWidget(&texture, 3, 1);

    QPushButton addButton("Add");
    connect(&addButton, &QPushButton::clicked, &actorSystemPopup, &QDialog::accept);
    grid->addWidget(&addButton, 4, 1);

    actorSystemPopup.setLayout(grid);

    int ret = actorSystemPopup.exec();
    if (ret != 0)
    {
        auto newActorSystem = new ActorSystem();
        newActorSystem->name = name.text().toStdString();
        newActorSystem->shaderName = shader.text().toStdString();
        newActorSystem->modelName = model.text().toStdString();
        newActorSystem->textureName = texture.text().toStdString();

        GetWorld()->AddActorSystem(newActorSystem);

        PopulateActorSystemList();
    }
}
