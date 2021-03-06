#pragma once

#include <qwidget.h>
#include <qlineedit.h>

class TransformEditWidget : public QLineEdit
{
public:
    TransformEditWidget(float val, QWidget* parent = 0);
};

