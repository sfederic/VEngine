#include "vpch.h"
#include "ToolbarDock.h"
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qlabel.h>
#include "PropertyWidgets/SpinBox.h"
#include "Core.h"
#include "TransformGizmo.h"
#include "FileSystem.h"
#include "Camera.h"
#include "Components/CameraComponent.h"
#include "WorldEditor.h"

ToolbarDock::ToolbarDock() : QDockWidget("Toolbar")
{
	auto hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setAlignment(Qt::AlignHCenter);

	//gameplay buttons
	playButton = new QPushButton(this);
	QPixmap playButtonIcon("Icons/start_play_icon.png");
	playButton->setIcon(playButtonIcon);
	playButton->setIconSize(QSize(30, 30));
	playButton->setToolTip("Ctrl+P");
	connect(playButton, &QPushButton::clicked, this, &ToolbarDock::StartPlay);
	hLayout->addWidget(playButton);

	resetWorldButton = new QPushButton(this);
	QPixmap resetWorldIcon("Icons/reset_icon.png");
	resetWorldButton->setIcon(resetWorldIcon);
	resetWorldButton->setIconSize(QSize(30, 30));
	resetWorldButton->setToolTip("Ctrl+R");
	connect(resetWorldButton, &QPushButton::clicked, this, &ToolbarDock::ResetWorldState);
	hLayout->addWidget(resetWorldButton);

	saveWorldButton = new QPushButton(this);
	QPixmap saveWorldIcon("Icons/save_icon.png");
	saveWorldButton->setIcon(saveWorldIcon);
	saveWorldButton->setIconSize(QSize(30, 30));
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
	cameraMoveSpeedSpinBox->setValue(editorCamera.cameraMoveSpeed);
	hLayout->addWidget(cameraMoveSpeedSpinBox);
	connect(cameraMoveSpeedSpinBox, &SpinBox::editingFinished,
		this, [&]() {editorCamera.cameraMoveSpeed = cameraMoveSpeedSpinBox->value(); });

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
