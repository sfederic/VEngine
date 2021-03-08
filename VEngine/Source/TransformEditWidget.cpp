#include "TransformEditWidget.h"
#include <QDoubleValidator>
#include <limits>

TransformEditWidget::TransformEditWidget(float val, QWidget* parent) : QDoubleSpinBox(parent)
{
    setDecimals(0);
    setMinimum(std::numeric_limits<float>::min());
    setMaximum(std::numeric_limits<float>::max());
    setValue(val);
}