#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EditorMainWindow.h"

class EditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EditorMainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::EditorMainWindowClass ui;
};
