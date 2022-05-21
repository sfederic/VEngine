#include "vpch.h"
#include "ToolbarDock.h"
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include "PropertyWidgets/SpinBox.h"
#include "Core.h"
#include "TransformGizmo.h"
#include "FileSystem.h"

ToolbarDock::ToolbarDock() : QDockWidget("Toolbar")
{
	auto hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setAlignment(Qt::AlignHCenter);


	//gameplay buttons
	playButton = new QPushButton("Play");
	connect(playButton, &QPushButton::clicked, this, &ToolbarDock::StartPlay);
	hLayout->addWidget(playButton);

	resetWorldButton = new QPushButton("Reset");
	connect(resetWorldButton, &QPushButton::clicked, this, &ToolbarDock::ResetWorldState);
	hLayout->addWidget(resetWorldButton);

	saveWorldButton = new QPushButton("Save");
	connect(saveWorldButton, &QPushButton::clicked, this, &ToolbarDock::SaveWorld);
	hLayout->addWidget(saveWorldButton);

	//TRANSFORMGIZMO SNAP INPUTS
	//Translation
	translationSnapSpinBox = new SpinBox(this);
	translationSnapSpinBox->setFixedWidth(75);
	translationSnapSpinBox->setValue(transformGizmo.translateSnapValues[0]);
	connect(translationSnapSpinBox, &SpinBox::editingFinished, this, &ToolbarDock::SetTranslationSnapValue);

	hLayout->addWidget(new QLabel("Translation:"));
	hLayout->addWidget(translationSnapSpinBox);

	//Scale
	scaleSnapSpinBox = new SpinBox(this);
	scaleSnapSpinBox->setFixedWidth(75);
	scaleSnapSpinBox->setValue(transformGizmo.scaleSnapValues[0]);
	connect(scaleSnapSpinBox, &SpinBox::editingFinished, this, &ToolbarDock::SetScaleSnapValue);

	hLayout->addWidget(new QLabel("Scale:"));
	hLayout->addWidget(scaleSnapSpinBox);

	//Rotation
	rotationSnapSpinBox = new SpinBox(this);
	rotationSnapSpinBox->setFixedWidth(75);
	rotationSnapSpinBox->setValue(transformGizmo.rotationSnapValues[0]);
	connect(rotationSnapSpinBox, &SpinBox::editingFinished, this, &ToolbarDock::SetRotationSnapValue);

	hLayout->addWidget(new QLabel("Rotation:"));
	hLayout->addWidget(rotationSnapSpinBox);


	auto toolbarWidget = new QWidget();
	toolbarWidget->setLayout(hLayout);

	setWidget(toolbarWidget);
}

void ToolbarDock::StartPlay()
{
	Core::SetGameplayState();

	if (Core::gameplayOn)
	{
		playButton->setText("Stop");
	}
	else
	{
		playButton->setText("Play");
	}
}

void ToolbarDock::ResetWorldState()
{
	FileSystem::ReloadCurrentWorld();
}

void ToolbarDock::SaveWorld()
{
	FileSystem::WriteAllActorSystems();
}

void ToolbarDock::SetTranslationSnapValue()
{
	float value = (float)translationSnapSpinBox->value();
	transformGizmo.SetTranslateSnapValue(value);
}

void ToolbarDock::SetScaleSnapValue()
{
	float value = (float)scaleSnapSpinBox->value();
	transformGizmo.SetScaleSnapValue(value);
}

void ToolbarDock::SetRotationSnapValue()
{
	float value = (float)rotationSnapSpinBox->value();
	transformGizmo.SetRotationSnapValue(value);
}
