#include "ToolbarDock.h"
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qdockwidget.h>
#include "ActorSystemFactory.h"
#include <qpushbutton.h>
#include <assert.h>
#include <qlineedit.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qcombobox.h>
#include "CoreSystem.h"
#include "../EditorMainWindow.h"

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

    //Play button
    QToolButton* playToolButton = new QToolButton();
    QPixmap playToolPixmap = QPixmap("play_icon.png");
    QIcon playToolIcon = QIcon(playToolPixmap);
    playToolButton->setIcon(playToolIcon);
    playToolButton->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextUnderIcon);
    playToolButton->setText("Play");
    connect(playToolButton, &QToolButton::clicked, this, &ToolbarDock::SetPlayMode);
    toolbarHLayout->addWidget(playToolButton, Qt::AlignLeft);

    //Pause button
    QToolButton* pauseToolButton = new QToolButton();
    pauseToolButton->setText("Pause");
    connect(pauseToolButton, &QToolButton::clicked, this, &ToolbarDock::PauseViewportInPlayMode);

    toolbarHLayout->addStretch(0);

    QWidget* toolbarWidget = new QWidget();
    toolbarWidget->setLayout(toolbarHLayout);

    setWidget(toolbarWidget);
    setMaximumHeight(90);
}

void ToolbarDock::Tick()
{

}

QDialog* createActorSystemWindow;
QLineEdit* actorSystemNameLineEdit;
QLineEdit* actorNameLineEdit;

void ToolbarDock::SetPlayMode()
{
    gCoreSystem.bGamePlayOn = !gCoreSystem.bGamePlayOn;

    if (gCoreSystem.bGamePlayOn)
    {
        gEditorSystem->DisableEditorDocks();
    }
    else if (!gCoreSystem.bGamePlayOn)
    {
        gEditorSystem->EnableEditorDocks();
    }
}

void ToolbarDock::PauseViewportInPlayMode()
{
    if (gCoreSystem.bGamePlayOn)
    {
        gCoreSystem.bGamePaused = !gCoreSystem.bGamePaused;
    }
}
