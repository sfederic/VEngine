#include "vpch.h"
#include "ToolbarDock.h"
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qlabel.h>
#include "PropertyWidgets/SpinBox.h"
#include "Core/Core.h"
#include "TransformGizmo.h"
#include "Core/FileSystem.h"
#include "Core/Camera.h"
#include "Components/CameraComponent.h"
#include "Core/WorldEditor.h"

ToolbarDock::ToolbarDock() : QDockWidget("Toolbar")
{
	auto hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setAlignment(Qt::AlignHCenter);

	//@Todo: there's something stupid on the buttons here where if you don't set the button text
	//or you set an icon on the button, the entire application won't get maximized on startup...
	//gameplay buttons
	playButton = new QPushButton("Play", this);
	playButton->setToolTip("Ctrl+P");
	connect(playButton, &QPushButton::clicked, this, &ToolbarDock::StartPlay);
	hLayout->addWidget(playButton);

	resetWorldButton = new QPushButton("Reset", this);
	resetWorldButton->setToolTip("Ctrl+R");
	connect(resetWorldButton, &QPushButton::clicked, this, &ToolbarDock::ResetWorldState);
	hLayout->addWidget(resetWorldButton);

	saveWorldButton = new QPushButton("Save", this);
	saveWorldButton->setToolTip("Ctrl+S");
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

	//World/Local transform
	worldLocalTransformSetting = new QComboBox(this);
	worldLocalTransformSetting->setFixedWidth(100);
	worldLocalTransformSetting->addItem("World");
	worldLocalTransformSetting->addItem("Local");
	connect(worldLocalTransformSetting, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
		this, &ToolbarDock::LocalWorldTransformValueChanged);
	hLayout->addWidget(worldLocalTransformSetting);

	//Camera movespeed
	hLayout->addWidget(new QLabel("Camera Speed:"));

	cameraMoveSpeedSpinBox = new SpinBox(this);
	cameraMoveSpeedSpinBox->setFixedWidth(75);
	cameraMoveSpeedSpinBox->setValue(Camera::GetActiveCamera().GetCameraMoveSpeed());
	hLayout->addWidget(cameraMoveSpeedSpinBox);
	connect(cameraMoveSpeedSpinBox, &SpinBox::editingFinished,
		this, [&]() { Camera::GetEditorCamera().SetCameraMoveSpeed(cameraMoveSpeedSpinBox->value()); });

	//Picker mode
	hLayout->addWidget(new QLabel("Pick Mode", this));

	worldEditorPickMode = new QComboBox(this);
	worldEditorPickMode->addItem(pickerStringActor);
	worldEditorPickMode->addItem(pickerStringComponent);
	connect(worldEditorPickMode, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
		this, &ToolbarDock::PickModeChanged);
	hLayout->addWidget(worldEditorPickMode);

	auto toolbarWidget = new QWidget();
	toolbarWidget->setLayout(hLayout);

	setWidget(toolbarWidget);
}

void ToolbarDock::SetTransformDisplay(std::string currentTransformSettingName)
{

}

void ToolbarDock::SetPlayButtonText()
{
	QString playIconText;

	if (Core::gameplayOn)
	{
		playIconText = "Pause";
	}
	else
	{
		playIconText = "Play";
	}

	playButton->setText(playIconText);
}

void ToolbarDock::StartPlay()
{
	Core::SetGameplayState();

	SetPlayButtonText();
}

void ToolbarDock::ResetWorldState()
{
	FileSystem::ReloadCurrentWorld();
}

void ToolbarDock::SaveWorld()
{
	FileSystem::SerialiseAllSystems();
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

void ToolbarDock::PickModeChanged(const QString& item)
{
	if (item == pickerStringActor)
	{
		WorldEditor::SetPickMode(WorldEditor::PickMode::Actor);
	}
	else if (item == pickerStringComponent)
	{
		WorldEditor::SetPickMode(WorldEditor::PickMode::Component);
	}
}

void ToolbarDock::LocalWorldTransformValueChanged(const QString& item)
{
	std::string transformMode = item.toStdString();
	transformGizmo.SetLocalWorldTransformMode(transformMode);
}
