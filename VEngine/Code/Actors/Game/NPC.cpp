#include "vpch.h"
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
}

void NPC::Start()
{
	__super::Start();

	if (!spawnText.empty())
	{
		if (isQuickDialogueActive) return;

		spawnTextWidget = UISystem::CreateWidget<DialogueWidget>();
		spawnTextWidget->dialogueText = spawnText;
		spawnTextWidget->AddToViewport(5.0f);
	}
}

void NPC::Tick(float deltaTime)
{
	if (spawnTextWidget)
	{
		spawnTextWidget->worldPosition = GetHomogeneousPositionV();
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

	TryToEscapeToExit();

	__super::Tick(deltaTime); //Tick has to be at the end here to deal with A* nodes
}

Properties NPC::GetProps()
{
	Properties props = __super::GetProps();
	props.title = "NPC";
	props.Add("Spawn Text", &spawnText);
	return props;
}

void NPC::QuickTalkTo()
{
	spawnTextWidget->RemoveFromViewport();

	if (isQuickDialogueActive) return;

	isQuickDialogueActive = true;

	dialogueComponent->dialogueWidget->dialogueText = interactText;
	dialogueComponent->AddToViewport();
}

void NPC::EndQuickTalkTo()
{
	isQuickDialogueActive = false;
	dialogueComponent->RemoveFromViewport();
}

void NPC::TryToEscapeToExit()
{
	auto entrance = EntranceTrigger::system.GetFirstActor();
	const int xIndex = (int)std::round(entrance->GetPosition().x);
	const int yIndex = (int)std::round(entrance->GetPosition().z);
	auto grid = Grid::system.GetOnlyActor();
	auto entranceNode = grid->GetNode(xIndex, yIndex);

	if (entranceReachableForEscape)
	{
		//Destroy NPC when it reaches the entrance.
		if (GetCurrentNode()->Equals(entranceNode))
		{
			DeferDestroy();
		}

		return;
	}

	MoveToNode(entranceNode);

	for (auto pathNode : pathNodes)
	{
		if (pathNode->Equals(entranceNode))
		{
			entranceReachableForEscape = true;
		}
	}

	if (!entranceReachableForEscape)
	{
		pathNodes.clear();
	}
}
