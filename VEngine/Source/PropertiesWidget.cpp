#include "PropertiesWidget.h"
#include "WorldEditor.h"
#include <qgridlayout.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qcheckbox.h>

#include "../EditorMainWindow.h"
#include <qpushbutton.h>
#include <qfiledialog.h>

void PropertiesWidget::SetActorPositionX()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && posEditX)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        pos.x = posEditX->text().toFloat();
        gWorldEditor.pickedActor->SetPosition(pos);
    }

    clearFocus();
}

void PropertiesWidget::SetActorPositionY()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && posEditY)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        pos.y = posEditY->text().toFloat();
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
        pos.z = posEditZ->text().toFloat();
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
        scale.x = scaleEditX->text().toFloat();
        gWorldEditor.pickedActor->SetScale(scale);
    }
}

void PropertiesWidget::SetActorScaleY()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && scaleEditY)
    {
        XMFLOAT3 scale = picked->GetScale();
        scale.y = scaleEditY->text().toFloat();
        gWorldEditor.pickedActor->SetScale(scale);
    }
}

void PropertiesWidget::SetActorScaleZ()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && scaleEditZ)
    {
        XMFLOAT3 scale = picked->GetScale();
        scale.z = scaleEditZ->text().toFloat();
        gWorldEditor.pickedActor->SetScale(scale);
    }
}

void PropertiesWidget::SetActorRotationX()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && rotEditX)
    {
        XMFLOAT3 rot = picked->GetRollPitchYaw();
        rot.x = rotEditX->text().toFloat();
        gWorldEditor.pickedActor->SetRotation(rot);
    }
}

void PropertiesWidget::SetActorRotationY()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && rotEditY)
    {
        XMFLOAT3 rot = picked->GetRollPitchYaw();
        rot.y = rotEditY->text().toFloat();
        gWorldEditor.pickedActor->SetRotation(rot);
    }
}

void PropertiesWidget::SetActorRotationZ()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && rotEditZ)
    {
        XMFLOAT3 rot = picked->GetRollPitchYaw();
        rot.z = rotEditZ->text().toFloat();
        gWorldEditor.pickedActor->SetRotation(rot);
    }
}

PropertiesWidget::PropertiesWidget(QWidget* parent) : QWidget(parent)
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    QGridLayout* grid = new QGridLayout();

    //Position
    QLabel* posLabel = new QLabel("Position");

    posEditX = new TransformEditWidget(0.f, this);
    connect(posEditX, &QLineEdit::returnPressed, this, &PropertiesWidget::SetActorPositionX);
    posEditY = new TransformEditWidget(0.f, this);
    connect(posEditY, &QLineEdit::returnPressed, this, &PropertiesWidget::SetActorPositionY);
    posEditZ = new TransformEditWidget(0.f, this);
    connect(posEditZ, &QLineEdit::returnPressed, this, &PropertiesWidget::SetActorPositionZ);

    grid->addWidget(posLabel, 0, 0);
    grid->addWidget(posEditX, 0, 1);
    grid->addWidget(posEditY, 0, 2);
    grid->addWidget(posEditZ, 0, 3);

    //Rotation
    rotEditX = new TransformEditWidget(0.0f, this);
    connect(rotEditX, &QLineEdit::returnPressed, this, &PropertiesWidget::SetActorRotationX);
    rotEditY = new TransformEditWidget(0.0f, this);
    connect(rotEditY, &QLineEdit::returnPressed, this, &PropertiesWidget::SetActorRotationY);
    rotEditZ = new TransformEditWidget(0.0f, this);
    connect(rotEditZ, &QLineEdit::returnPressed, this, &PropertiesWidget::SetActorRotationZ);

    QLabel* rotLabel = new QLabel("Rotation");

    grid->addWidget(rotLabel, 1, 0);
    grid->addWidget(rotEditX, 1, 1);
    grid->addWidget(rotEditY, 1, 2);
    grid->addWidget(rotEditZ, 1, 3);

    //Scale
    scaleEditX = new TransformEditWidget(0.0f, this);
    connect(scaleEditX, &QLineEdit::returnPressed, this, &PropertiesWidget::SetActorScaleX);
    scaleEditY = new TransformEditWidget(0.0f, this);
    connect(scaleEditY, &QLineEdit::returnPressed, this, &PropertiesWidget::SetActorScaleY);
    scaleEditZ = new TransformEditWidget(0.0f, this);
    connect(scaleEditZ, &QLineEdit::returnPressed, this, &PropertiesWidget::SetActorScaleZ);


    QLabel* scaleLabel = new QLabel("Scale");

    grid->addWidget(scaleLabel, 2, 0);
    grid->addWidget(scaleEditX, 2, 1);
    grid->addWidget(scaleEditY, 2, 2);
    grid->addWidget(scaleEditZ, 2, 3);

    grid->setAlignment(Qt::AlignTop);

    QVBoxLayout* vLayoutTop = new QVBoxLayout();

    vLayoutTop->addLayout(grid);


    //Actor/ActorSystem Properties
    QGridLayout* actorSystemDetailsGrid = new QGridLayout();

    QCheckBox* checkBox = new QCheckBox();
    checkBox->setText("Renderable");
    actorSystemDetailsGrid->addWidget(checkBox, 0, 0);

    //Line edits 
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

    vLayoutTop->addLayout(actorSystemDetailsGrid);

    setLayout(vLayoutTop);
}

void PropertiesWidget::SelectShader()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Shaders"), "Shaders", tr("Shader Files (*.hlsl)"));

    if (!fileName.isEmpty())
    {
        //QFileInfo here chops of the entire path, leaves only the filename.
        actorSystemShaderName->setText(QFileInfo(fileName).fileName());
    }
}

void PropertiesWidget::SelectModel()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
        tr("Open Models"), "Models", tr("Model Files (*.fbx *.obj)"));

    if (!fileName.isEmpty())
    {
        actorSystemModelName->setText(QFileInfo(fileName).fileName());
    }
}

void PropertiesWidget::SelectTexture()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Textures"), "Textures", tr("Texture Files (*.jpg *.png)"));

    if (!fileName.isEmpty())
    {
        actorSystemTextureName->setText(QFileInfo(fileName).fileName());
    }
}

void PropertiesWidget::Tick()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (posEditX->hasFocus())
    {
        return;
    }
    else if (picked)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        posEditX->setText(QString::number(pos.x));
        posEditY->setText(QString::number(pos.y));
        posEditZ->setText(QString::number(pos.z));

        XMFLOAT3 rot = picked->GetRollPitchYaw();
        rotEditX->setText(QString::number(rot.x));
        rotEditY->setText(QString::number(rot.y));
        rotEditZ->setText(QString::number(rot.z));

        XMFLOAT3 scale = picked->GetScale();
        scaleEditX->setText(QString::number(scale.x));
        scaleEditY->setText(QString::number(scale.y));
        scaleEditZ->setText(QString::number(scale.z));
    }
}