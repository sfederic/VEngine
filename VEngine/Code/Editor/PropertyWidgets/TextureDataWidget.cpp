#include "TextureDataWidget.h"
#include <qpushbutton.h>
#include <qfiledialog.h>
#include "Render/PipelineObjects.h"
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
	//BUG: is there a fucking bug when using the OS' native file opener?
	//https://stackoverflow.com/questions/31983412/code-freezes-on-trying-to-open-qdialog
	QString filepath = QFileDialog::getOpenFileName(this,
		tr("Open Image"), "Textures/", tr("Image Files (*.png *.jpg *.bmp)"),
		0, QFileDialog::DontUseNativeDialog);

	QFileInfo info(filepath);
	QString filename = info.fileName();

	if (!filename.isEmpty())
	{
		value->filename = filename.toStdString();
		prop.change(value);
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
