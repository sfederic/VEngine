#include "ToolbarDock.h"
#include <qpushbutton.h>
#include <qboxlayout.h>
#include "Core.h"

ToolbarDock::ToolbarDock() : QDockWidget("Toolbar")
{
	auto hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setAlignment(Qt::AlignHCenter);

	playButton = new QPushButton("Play");
	connect(playButton, &QPushButton::clicked, this, &ToolbarDock::StartPlay);
	hLayout->addWidget(playButton);

	pauseButton = new QPushButton("Pause");
	hLayout->addWidget(pauseButton);

	auto toolbarWidget = new QWidget();
	toolbarWidget->setLayout(hLayout);

	setWidget(toolbarWidget);
}

void ToolbarDock::StartPlay()
{
	Core::gameplayOn = !Core::gameplayOn;

	if (Core::gameplayOn)
	{
		playButton->setText("Stop");
	}
	else
	{
		playButton->setText("Play");
	}
}
