#include "ToolbarDock.h"
#include <qpushbutton.h>
#include <qboxlayout.h>

ToolbarDock::ToolbarDock() : QDockWidget("Toolbar")
{
	auto hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setAlignment(Qt::AlignHCenter);

	auto playButton = new QPushButton("Play");
	//playButton->setMaximumWidth(150);
	hLayout->addWidget(playButton);

	auto pauseButton = new QPushButton("Pause");
	//pauseButton->setMaximumWidth(150);
	hLayout->addWidget(pauseButton);

	auto toolbarWidget = new QWidget();
	toolbarWidget->setLayout(hLayout);

	setWidget(toolbarWidget);
}
