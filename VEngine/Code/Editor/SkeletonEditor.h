#pragma once
#include <qwidget.h>

class QTreeWidget;
class QTreeWidgetItem;
struct Joint;

//@Todo: not working 100%, there's a base to work from here if the engine needs it.
struct SkeletonEditor : public QWidget
{
	QTreeWidget* boneTreeWidget = nullptr;

	SkeletonEditor(QWidget* parent = nullptr);
	void CreateBoneTreeNode(Joint& joint, QTreeWidgetItem* parentNode = nullptr);
};
