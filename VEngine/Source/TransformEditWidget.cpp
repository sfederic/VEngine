#include "TransformEditWidget.h"
#include <QDoubleValidator>
#include <limits>

TransformEditWidget::TransformEditWidget(float val, QWidget* parent) : QDoubleSpinBox(parent)
{
    setDecimals(4);
    setMinimum(std::numeric_limits<float>::lowest());
    setMaximum(std::numeric_limits<double>::max());
    setValue(val);
}