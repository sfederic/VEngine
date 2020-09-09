#include "TransformEditWidget.h"
#include <QDoubleValidator>
#include <d3d11.h> //TODO: change the FLOAT32_MAX

TransformEditWidget::TransformEditWidget(float val, QWidget* parent) : QLineEdit(parent)
{
    setValidator(new QDoubleValidator(-D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX, 32, parent));
    setText(QString::number(val));
}