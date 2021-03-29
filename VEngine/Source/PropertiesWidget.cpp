#include "PropertiesWidget.h"
#include "WorldEditor.h"
#include <qgridlayout.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include "MathHelpers.h"
#include "../EditorMainWindow.h"
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <QDoubleSpinBox>
#include <QLineEdit>

void PropertiesWidget::SetActorPositionX()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && posEditX)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        pos.x = posEditX->value();
        gWorldEditor.pickedActor->SetPosition(pos);
    }

    //clearFocus();
}

void PropertiesWidget::SetActorPositionY()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && posEditY)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        pos.y = posEditY->value();
        gWorldEditor.pickedActor->SetPosition(pos);
    }

    clearFocus();
}

void PropertiesWidget::SetActorPositionZ()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && posEditZ)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        pos.z = posEditZ->value();
        gWorldEditor.pickedActor->SetPosition(pos);
    }

    clearFocus();
}

void PropertiesWidget::SetActorScaleX()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && scaleEditX)
    {
        XMFLOAT3 scale = picked->GetScale();
        scale.x = scaleEditX->value();
        gWorldEditor.pickedActor->SetScale(scale);
    }
}

void PropertiesWidget::SetActorScaleY()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && scaleEditY)
    {
        XMFLOAT3 scale = picked->GetScale();
        scale.y = scaleEditY->value();
        gWorldEditor.pickedActor->SetScale(scale);
    }
}

void PropertiesWidget::SetActorScaleZ()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && scaleEditZ)
    {
        XMFLOAT3 scale = picked->GetScale();
        scale.z = scaleEditZ->value();
        gWorldEditor.pickedActor->SetScale(scale);
    }
}

void PropertiesWidget::SetActorRotationX()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && rotEditX)
    {
        XMFLOAT3 euler = picked->GetPitchYawRoll();
        euler.x = rotEditX->value();
        gWorldEditor.pickedActor->SetRotation(euler);
    }
}

void PropertiesWidget::SetActorRotationY()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && rotEditY)
    {
        XMFLOAT3 euler = picked->GetPitchYawRoll();
        euler.y = rotEditY->value();
        gWorldEditor.pickedActor->SetRotation(euler);
    }
}

void PropertiesWidget::SetActorRotationZ()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && rotEditZ)
    {
        XMFLOAT3 euler = picked->GetPitchYawRoll();
        euler.z = rotEditZ->value();
        gWorldEditor.pickedActor->SetRotation(euler);
    }
}

PropertiesWidget::PropertiesWidget(QWidget* parent) : QWidget(parent)
{
    QGridLayout* grid = new QGridLayout();

    //Position
    QLabel* posLabel = new QLabel("Position");

    posEditX = new TransformEditWidget(0.f, this);
    //Note: QDoubleSpinBox::editingFinished calls its calling function twice for some reason.
    connect(posEditX, &QDoubleSpinBox::editingFinished, this, &PropertiesWidget::SetActorPositionX);
    posEditY = new TransformEditWidget(0.f, this);
    connect(posEditY, &QDoubleSpinBox::editingFinished, this, &PropertiesWidget::SetActorPositionY);
    posEditZ = new TransformEditWidget(0.f, this);
    connect(posEditZ, &QDoubleSpinBox::editingFinished, this, &PropertiesWidget::SetActorPositionZ);

    QLabel* xLabel = new QLabel("X");
    xLabel->setStyleSheet("border: 1px solid red;");
    QLabel* yLabel = new QLabel("Y");
    yLabel->setStyleSheet("border: 1px solid green;");
    QLabel* zLabel = new QLabel("Z");
    zLabel->setStyleSheet("border: 1px solid blue;");

    grid->addWidget(xLabel, 0, 1);
    grid->addWidget(yLabel, 0, 2);
    grid->addWidget(zLabel, 0, 3);

    grid->addWidget(posLabel, 1, 0);
    grid->addWidget(posEditX, 1, 1);
    grid->addWidget(posEditY, 1, 2);
    grid->addWidget(posEditZ, 1, 3);

    //Rotation
    rotEditX = new TransformEditWidget(0.0f, this);
    connect(rotEditX, &QDoubleSpinBox::editingFinished, this, &PropertiesWidget::SetActorRotationX);
    rotEditY = new TransformEditWidget(0.0f, this);
    connect(rotEditY, &QDoubleSpinBox::editingFinished, this, &PropertiesWidget::SetActorRotationY);
    rotEditZ = new TransformEditWidget(0.0f, this);
    connect(rotEditZ, &QDoubleSpinBox::editingFinished, this, &PropertiesWidget::SetActorRotationZ);

    QLabel* rotLabel = new QLabel("Rotation");

    grid->addWidget(rotLabel, 2, 0);
    grid->addWidget(rotEditX, 2, 1);
    grid->addWidget(rotEditY, 2, 2);
    grid->addWidget(rotEditZ, 2, 3);

    //Scale
    scaleEditX = new TransformEditWidget(0.0f, this);
    connect(scaleEditX, &QDoubleSpinBox::editingFinished, this, &PropertiesWidget::SetActorScaleX);
    scaleEditY = new TransformEditWidget(0.0f, this);
    connect(scaleEditY, &QDoubleSpinBox::editingFinished, this, &PropertiesWidget::SetActorScaleY);
    scaleEditZ = new TransformEditWidget(0.0f, this);
    connect(scaleEditZ, &QDoubleSpinBox::editingFinished, this, &PropertiesWidget::SetActorScaleZ);


    QLabel* scaleLabel = new QLabel("Scale");

    grid->addWidget(scaleLabel, 3, 0);
    grid->addWidget(scaleEditX, 3, 1);
    grid->addWidget(scaleEditY, 3, 2);
    grid->addWidget(scaleEditZ, 3, 3);

    grid->setAlignment(Qt::AlignTop);

    QVBoxLayout* vLayoutTop = new QVBoxLayout();

    vLayoutTop->addLayout(grid);

    //Actor/ActorSystem Properties
    QGridLayout* actorSystemDetailsGrid = new QGridLayout();

    QGridLayout* actorDetailsGrid = new QGridLayout();

    QCheckBox* checkBox = new QCheckBox();
    checkBox->setText("Renderable");
    checkBox->setChecked(true);
    actorSystemDetailsGrid->addWidget(checkBox, 0, 0);
    connect(checkBox, &QCheckBox::clicked, this, &PropertiesWidget::SetActorRenderable);

    //Actor properties
    actorDetailsGrid->addWidget(new QLabel("Actor Name"), 0, 0);
    actorName = new QLineEdit();
    actorDetailsGrid->addWidget(actorName, 0, 1);

    //Actorsystem properties
    actorSystemName = new QLineEdit();
    actorSystemName->setReadOnly(true);
    actorSystemDetailsGrid->addWidget(actorSystemName, 1, 0);

    actorSystemModelName = new QLineEdit();
    actorSystemModelName->setReadOnly(true);
    actorSystemDetailsGrid->addWidget(actorSystemModelName, 2, 0);

    actorSystemShaderName = new QLineEdit();
    actorSystemShaderName->setReadOnly(true);
    actorSystemDetailsGrid->addWidget(actorSystemShaderName, 3, 0);

    actorSystemTextureName = new QLineEdit();
    actorSystemTextureName->setReadOnly(true);
    actorSystemDetailsGrid->addWidget(actorSystemTextureName, 4, 0);

    //Open file dialog buttons (These are fine to be defined locally like this)
    QPushButton* selectModelButton = new QPushButton("Select Model");
    connect(selectModelButton, &QPushButton::pressed, this, &PropertiesWidget::SelectModel);
    actorSystemDetailsGrid->addWidget(selectModelButton, 2, 1);

    QPushButton* selectShaderButton = new QPushButton("Select Shader");
    connect(selectShaderButton, &QPushButton::pressed, this, &PropertiesWidget::SelectShader);
    actorSystemDetailsGrid->addWidget(selectShaderButton, 3, 1);

    QPushButton* selectTextureButton = new QPushButton("Select Texture");
    connect(selectTextureButton, &QPushButton::pressed, this, &PropertiesWidget::SelectTexture);
    actorSystemDetailsGrid->addWidget(selectTextureButton, 4, 1);

    vLayoutTop->addLayout(actorDetailsGrid);
    vLayoutTop->addLayout(actorSystemDetailsGrid);

    setLayout(vLayoutTop);
}

void PropertiesWidget::SelectShader()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Shaders"), "Shaders", tr("Shader Files (*.hlsl)"));

    if (!filePath.isEmpty())
    {
        QString fileName = QFileInfo(filePath).fileName();
        actorSystemShaderName->setText(QFileInfo(fileName).fileName());

        selectedActorSystem->shaderName = fileName.toStdWString();
    }
}

void PropertiesWidget::SelectModel()
{
    QString filePath = QFileDialog::getOpenFileName(this, 
        tr("Open Models"), "Models", tr("Model Files (*.fbx *.obj)"));

    if (!filePath.isEmpty())
    {
        QString fileName = QFileInfo(filePath).fileName();
        actorSystemModelName->setText(fileName);

        selectedActorSystem->modelName = fileName.toStdString();
        selectedActorSystem->RecreateModel();
    }
}

void PropertiesWidget::SelectTexture()
{
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Open Textures"), "Textures", tr("Texture Files (*.jpg *.png)"));

    if (!filePath.isEmpty())
    {
        QString fileName = QFileInfo(filePath).fileName();
        actorSystemTextureName->setText(fileName);

        selectedActorSystem->textureName = fileName.toStdWString();
        selectedActorSystem->RecreateTexture();
    }
}

void PropertiesWidget::Tick()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        posEditX->setValue(pos.x);
        posEditY->setValue(pos.y);
        posEditZ->setValue(pos.z);

        XMFLOAT3 rot;
        rotEditX->setValue(rot.x);
        rotEditY->setValue(rot.y);
        rotEditZ->setValue(rot.z);

        XMFLOAT3 scale = picked->GetScale();
        scaleEditX->setValue(scale.x);
        scaleEditY->setValue(scale.y);
        scaleEditZ->setValue(scale.z);
    }
}

void PropertiesWidget::SetActorRenderable(bool renderCheck)
{
    selectedActorSystem->GetActor(0)->bRender = renderCheck;
}