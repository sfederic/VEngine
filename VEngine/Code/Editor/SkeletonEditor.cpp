#include "SkeletonEditor.h"
#include <map>
#include <cassert>
#include <qtreewidget.h>
#include "WorldEditor.h"
#include "Actors/Actor.h"
#include "Components/MeshComponent.h"

SkeletonEditor::SkeletonEditor(QWidget* parent) : QWidget(parent)
{
	boneTreeWidget = new QTreeWidget(this);
	boneTreeWidget->setWindowTitle("Skeleton Editor");
	boneTreeWidget->setHeaderLabel("Bones");

	auto meshes = worldEditor.pickedActor->GetComponentsOfType<MeshComponent>();
	auto mesh = meshes.front();
	auto skeleton = mesh->GetSkeleton();

	std::vector<Joint*> joints;
	for (Joint& joint : skeleton->joints)
	{
		joints.push_back(&joint);
	}

	for (Joint* parent : joints)
	{
		CreateBoneTreeNode(*parent);

		for (Joint* child : joints)
		{
			if (child->parentIndex == parent->index)
			{
				auto items = boneTreeWidget->findItems(QString::fromStdString(parent->name),
					Qt::MatchFlag::MatchExactly);
				assert(items.count() == 1);
				CreateBoneTreeNode(*child, items[0]);
			}
		}
	}
}

void SkeletonEditor::CreateBoneTreeNode(Joint& joint, QTreeWidgetItem* parentNode)
{
	QTreeWidgetItem* item = nullptr;

	if (parentNode) item = new QTreeWidgetItem(parentNode);
	else item = new QTreeWidgetItem(boneTreeWidget);

	item->setText(0, QString::fromStdString(joint.name));
}
