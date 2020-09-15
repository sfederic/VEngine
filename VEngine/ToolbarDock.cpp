#include "ToolbarDock.h"
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qdockwidget.h>

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

    toolbarHLayout->addStretch(0);

    QWidget* toolbarWidget = new QWidget();
    toolbarWidget->setLayout(toolbarHLayout);

    setWidget(toolbarWidget);
    setMaximumHeight(90);
}