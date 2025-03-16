
#include "NPC.h"
#include "EntranceTrigger.h"
#include "Grid.h"
#include "UI/Game/DialogueWidget.h"
#include "UI/UISystem.h"
#include "Components/Game/DialogueComponent.h"

NPC::NPC()
{
	isDestructible = true;
}

void NPC::Create()
{
	__super::Create();

	moveSpeed = 3.f;
	rotateSpeed = 18.f;

	canBeMovedInLink = false;
	canBeRotatedInLink = false;

	SetMeshFilename("char.vmesh");
}

void NPC::Start()
{
	__super::Start();

	if (!spawnText.empty())
	{
		if (isQuickDialogueActive) return;

		spawnTextWidget = UISystem::CreateWidget<DialogueWidget>();
		spawnTextWidget->SetText(spawnText);
		spawnTextWidget->AddToViewport(5.0f);
	}
}

void NPC::Tick(float deltaTime)
{
	if (spawnTextWidget)
	{
		spawnTextWidget->SetWorldPosition(GetHomogeneousPositionV());
	}

	if (isQuickDialogueActive)
	{
		quickTalkTimer += deltaTime;
		if (quickTalkTimer > 4.0f)
		{
			EndQuickTalkTo();
			quickTalkTimer = 0.f;
		}
	}

	TryToReachTargetNode();

	__super::Tick(deltaTime); //Tick has to be at the end here to deal with A* nodes
}

Properties NPC::GetProps()
{
	Properties props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Spawn Text", &spawnText);
	props.Add("Target Node X", &nodeToMoveToXIndex);
	props.Add("Target Node Y", &nodeToMoveToYIndex);
	props.Add("Reach TargetNode", &tryToReachTargetNode);
	return props;
}

void NPC::QuickTalkTo()
{
	spawnTextWidget->RemoveFromViewport();

	if (isQuickDialogueActive) return;

	isQuickDialogueActive = true;

	dialogueComponent->dialogueWidget->SetText(interactText);
	dialogueComponent->AddToViewport();
}

void NPC::EndQuickTalkTo()
{
	isQuickDialogueActive = false;
	dialogueComponent->RemoveFromViewport();
}

void NPC::TryToReachTargetNode()
{
	if (!tryToReachTargetNode)
	{
		return;
	}

	auto grid = Grid::system.GetOnlyActor();
	auto targetNode = grid->GetNode(nodeToMoveToXIndex, nodeToMoveToYIndex);

	if (canReachTargetNode)
	{
		if (GetCurrentNode()->Equals(targetNode))
		{
			Log("Target reached.");
			pathNodes.clear();
			tryToReachTargetNode = false;
		}
	}

	MoveToNode(targetNode);

	for (auto pathNode : pathNodes)
	{
		if (pathNode->Equals(targetNode))
		{
			canReachTargetNode = true;
		}
	}

	if (!canReachTargetNode)
	{
		pathNodes.clear();
	}
}
