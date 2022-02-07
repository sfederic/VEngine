#include "TextureDataWidget.h"
#include <qfiledialog.h>
#include "Render/RenderTypes.h"

TextureDataWidget::TextureDataWidget(Property prop_)
{
	prop = prop_;
	value = (TextureData*)prop.data;

	setText(QString::fromStdString(value->filename));
	connect(this, &QPushButton::clicked, this, &TextureDataWidget::SetValue);
}

void TextureDataWidget::SetValue()
{
	//BUG: You have to call CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) for the native file
	//dialog to work properly.
	QString filepath = QFileDialog::getOpenFileName(this,
		tr("Set Texture"), "Textures/", tr("Image Files (*.png *.jpg *.bmp)"));

	QFileInfo info(filepath);
	QString filename = info.fileName();

	if (!filename.isEmpty())
	{
		value->filename.assign(filename.toStdString().c_str());

		if (prop.change)
		{
			prop.change(value);
		}

		ResetValue();
	}
}

void TextureDataWidget::ResetValue()
{
	if (value)
	{
		setText(QString::fromStdString(value->filename));
	}
}
