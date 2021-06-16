#pragma once

#include <qwidget.h>
#include <QDoubleSpinBox>

class TransformEditWidget : public QDoubleSpinBox
{
public:
    TransformEditWidget(float value_, QWidget* parent = 0);

private: 
    //float* value;
};
