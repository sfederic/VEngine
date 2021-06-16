#pragma once

#include <qwidget.h>

class QLineEdit;
class ActorSystem;
class QVBoxLayout;

class PropertiesWidget : public QWidget
{
public:
    QLineEdit* actorName;

    QLineEdit* actorSystemName;
    QLineEdit* actorSystemShaderName;
    QLineEdit* actorSystemTextureName;
    QLineEdit* actorSystemModelName;
    QLineEdit* actorSystemMaterial;

    QVBoxLayout* entireVLayout;

    ActorSystem* selectedActorSystem = nullptr;

    PropertiesWidget(QWidget* parent = 0);

    void Tick();

    void SetActorRenderable(bool renderCheck);

    void SelectShader();
    void SelectTexture();
    void SelectModel();

    void SetActorPosition();
    void SetActorScale();
    void SetActorRotation();
    void SetActorName();
};
