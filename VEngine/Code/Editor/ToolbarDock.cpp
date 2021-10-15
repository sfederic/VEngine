#include "ToolbarDock.h"
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include "PropertyWidgets/SpinBox.h"
#include "Core.h"
#include "TransformGizmo.h"

ToolbarDock::ToolbarDock() : QDockWidget("Toolbar")
{
	auto hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setAlignment(Qt::AlignHCenter);


	//Play buttons
	playButton = new QPushButton("Play");
	connect(playButton, &QPushButton::clicked, this, &ToolbarDock::StartPlay);
	hLayout->addWidget(playButton);

	pauseButton = new QPushButton("Pause");
	hLayout->addWidget(pauseButton);


	//TRANSFORMGIZMO SNAP INPUTS
	//Translation
	translationSnapSpinBox = new SpinBox(this);
	translationSnapSpinBox->setValue(transformGizmo.translateSnapValues[0]);
	connect(translationSnapSpinBox, &SpinBox::editingFinished, this, &ToolbarDock::SetTranslationSnapValue);

	hLayout->addWidget(new QLabel("Translation:"));
	hLayout->addWidget(translationSnapSpinBox);

	//Scale
	scaleSnapSpinBox = new SpinBox(this);
	scaleSnapSpinBox->setValue(transformGizmo.scaleSnapValues[0]);
	connect(scaleSnapSpinBox, &SpinBox::editingFinished, this, &ToolbarDock::SetScaleSnapValue);

	hLayout->addWidget(new QLabel("Scale:"));
	hLayout->addWidget(scaleSnapSpinBox);

	//Rotation
	rotationSnapSpinBox = new SpinBox(this);
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
