#pragma once

#include <qwidget.h>
#include <QDoubleSpinBox>

class TransformEditWidget : public QDoubleSpinBox
{
public:
    TransformEditWidget(float val, QWidget* parent = 0);
};

