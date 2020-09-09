#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EditorMainWindow.h"
#include "RenderViewWidget.h"

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorMainWindow(QWidget *parent = Q_NULLPTR);
    MainWidget mainWidget;
    class PropertiesWidget* propWidget;
private:
    Ui::EditorMainWindowClass ui;
};
