#include "ToolbarDock.h"
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qdockwidget.h>
#include "ActorSystemFactory.h"
#include <qpushbutton.h>
#include <assert.h>

ToolbarDock::ToolbarDock(const char* title) : QDockWidget(title)
{
    QHBoxLayout* toolbarHLayout = new QHBoxLayout();

    QToolButton* saveToolButton = new QToolButton();
    QPixmap saveToolPixmap = QPixmap("test.png");
    QIcon saveToolIcon = QIcon(saveToolPixmap);
    saveToolButton->setIcon(saveToolIcon);
    saveToolButton->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextUnderIcon);
    saveToolButton->setText("Save");
    toolbarHLayout->addWidget(saveToolButton, Qt::AlignLeft);

    QToolButton* playToolButton = new QToolButton();
    QPixmap playToolPixmap = QPixmap("play_icon.png");
    QIcon playToolIcon = QIcon(playToolPixmap);
    playToolButton->setIcon(playToolIcon);
    playToolButton->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextUnderIcon);
    playToolButton->setText("Play");
    toolbarHLayout->addWidget(playToolButton, Qt::AlignLeft);

    //Add Actorsystem button
    QPushButton* createActorSystemButton = new QPushButton();
    createActorSystemButton->setText("New Actor System");
    connect(createActorSystemButton, &QToolButton::clicked, this, &ToolbarDock::CreateNewActorSystem);
    toolbarHLayout->addWidget(createActorSystemButton);

    toolbarHLayout->addStretch(0);

    QWidget* toolbarWidget = new QWidget();
    toolbarWidget->setLayout(toolbarHLayout);

    setWidget(toolbarWidget);
    setMaximumHeight(90);
}

void ToolbarDock::Tick()
{

}

void ToolbarDock::CreateNewActorSystem()
{
    const char* actorSystemHeaderFile =
        "#pragma once\n"
        "\n"
        "#include \"Actor.h\"\n"
        "\n"
        "class AActor : public Actor\n"
        "{\n"
        "public:\n"
        "\tAActor();\n"
        "};\n"
        "\n"
        "class AActorSystem : public ActorSystem\n"
        "{\n"
        "public:\n"
        "\tvirtual void Tick(float deltaTime) override;\n"
        "\tvirtual void SpawnActors(int numToSpawn);\n"
        "\tvirtual void SpawnActor(Transform transform);\n"
        "};\n"
        "\n"
        "extern AActorSystem aActorSystem;\n";

    const char* actorSystemSourceFile =
        "#include \"AActor.h\"\n"
        "#include \"ActorSystemFactory.h\"\n"
        "\n"
        "AActorSystem aActorSystem;"
        "\n"
        "TestActorSystem::TestActorSystem()\n"
        "{\n"
        "\tshaderName = L\"shaders.hlsl\";\n"
        "\ttextureName = L\"texture2.jpg\";\n"
        "\tmodelName = \"cube.fbx\";\n"
        "\tname = L\"testactor\";\n"
        "\n"
        "\tsizeofActor = sizeof(TestActor);\n"
        "\n"
        "\tActorSystemFactory::Register<TestActorSystem>(this);\n"
        "}\n"
        "\n"
        "void TestActorSystem::Tick(float deltaTime)\n"
        "{\n"
        "\n"
        "}\n"
        "\n"
        "void TestActorSystem::SpawnActors(int numToSpawn)\n"
        "{\n"
        "\tInit<TestActor>(&gRenderSystem, numToSpawn);\n"
        "}\n"
        "\n"
        "void TestActorSystem::SpawnActor(Transform transform)\n"
        "{\n"
        "\tAddActor<TestActor>(transform);\n"
        "}\n"
        "\n";

    FILE* file;

    //Write .h file.
    fopen_s(&file, "AActor.h", "w");
    assert(file);
    fprintf(file, "%s", actorSystemHeaderFile);
    fflush(file);

    //Write .cpp file.
    fopen_s(&file, "AActor.cpp", "w");
    assert(file);
    fprintf(file, "%s", actorSystemSourceFile);
    fflush(file);

    fclose(file);
}
