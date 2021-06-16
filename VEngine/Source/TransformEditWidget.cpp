#include "TransformEditWidget.h"
#include <QDoubleValidator>
#include <limits>

TransformEditWidget::TransformEditWidget(float value_, QWidget* parent) : QDoubleSpinBox(parent)
{
    setDecimals(4);
    setMinimum(std::numeric_limits<float>::lowest());
    setMaximum(std::numeric_limits<double>::max());
    setButtonSymbols(QAbstractSpinBox::NoButtons);

    //value = value_;

    setValue(value_);
}