#include "PropertiesDock.h"
#include <qwidget.h>
#include <qboxlayout.h>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>

PropertiesDock::PropertiesDock() : QDockWidget("Properties")
{
	auto grid = new QGridLayout();
	grid->setAlignment(Qt::AlignTop);

	grid->addWidget(new QLabel("Transform"), 0, 1, 1, 3);

	grid->addWidget(new QLabel("Position"), 1, 0);
	grid->addWidget(new QDoubleSpinBox(), 1, 1);
	grid->addWidget(new QDoubleSpinBox(), 1, 2);
	grid->addWidget(new QDoubleSpinBox(), 1, 3);

	grid->addWidget(new QLabel("Rotation"), 2, 0);
	grid->addWidget(new QDoubleSpinBox(), 2, 1);
	grid->addWidget(new QDoubleSpinBox(), 2, 2);
	grid->addWidget(new QDoubleSpinBox(), 2, 3);

	grid->addWidget(new QLabel("Scale"), 3, 0);
	grid->addWidget(new QDoubleSpinBox(), 3, 1);
	grid->addWidget(new QDoubleSpinBox(), 3, 2);
	grid->addWidget(new QDoubleSpinBox(), 3, 3);

	grid->addWidget(new QLabel("MeshComponent"), 4, 1, 1, 3);

	grid->addWidget(new QLabel("Shadow Depth"), 5, 0);
	grid->addWidget(new QCheckBox(), 5, 1, 1, 3);

	grid->addWidget(new QLabel("Name"), 6, 0);
	grid->addWidget(new QLineEdit(), 6, 1, 1, 3);


	auto propsWidget = new QWidget();
	propsWidget->setLayout(grid);

	setWidget(propsWidget);
}
