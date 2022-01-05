#include "MeshComponentDataWidget.h"
#include <qfiledialog.h>
#include "Render/RenderTypes.h"

MeshComponentDataWidget::MeshComponentDataWidget(Property prop_)
{
	prop = prop_;
	value = (MeshComponentData*)prop.data;

	setText(QString::fromStdString(value->filename));
	connect(this, &QPushButton::clicked, this, &MeshComponentDataWidget::SetValue);
}

void MeshComponentDataWidget::SetValue()
{
	QString filepath = QFileDialog::getOpenFileName(this,
		tr("Set Mesh"), "Meshes/", tr("Meshes (*.fbx *.vmesh)"));

	QFileInfo info(filepath);
	QString filename = info.fileName();

	if (!filename.isEmpty())
	{
		value->filename.assign(filename.toStdString().c_str());
		prop.change(value);
		ResetValue();
	}
}

void MeshComponentDataWidget::ResetValue()
{
	if (value)
	{
		setText(QString::fromStdString(value->filename));
	}
}
