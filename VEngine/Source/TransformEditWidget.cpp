#include "TransformEditWidget.h"
#include <QDoubleValidator>
#include <limits>

TransformEditWidget::TransformEditWidget(float val, QWidget* parent) : QLineEdit(parent)
{
    setValidator(new QDoubleValidator(std::numeric_limits<float>::min(),
        std::numeric_limits<float>::max(), 32, parent));
    setText(QString::number(val));
}