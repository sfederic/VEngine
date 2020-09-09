#pragma once

#include <qwidget.h>
#include <qlineedit.h>
#include <QListWidget>

class WorldWidget : public QWidget
{
public:
    void Tick();
    void ClickOnListActor(QListWidgetItem* listItem);
    WorldWidget(QWidget* parent = 0);
    void searchWorldList();

    QListWidget* worldList;
    QLineEdit* worldSearch;
    QStringList worldStringList;
};

