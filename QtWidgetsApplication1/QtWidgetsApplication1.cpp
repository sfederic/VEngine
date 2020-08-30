#include "QtWidgetsApplication1.h"

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    QAction* exit = new QAction("Exit", this);

    QMenu* menu = menuBar()->addMenu("File");
    menu->addAction(exit);
    connect(exit, &QAction::triggered, this, &QtWidgetsApplication1::close);
}
