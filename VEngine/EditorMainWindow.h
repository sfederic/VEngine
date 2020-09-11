#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EditorMainWindow.h"
#include "RenderViewWidget.h"
#include "PropertiesWidget.h"

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorMainWindow(QWidget *parent = Q_NULLPTR);
    MainWidget mainWidget;
    PropertiesWidget* propWidget;
private:
    Ui::EditorMainWindowClass ui;
};

