#include "IntuitionComponent.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Log.h"
#include "World.h"
#include "Gameplay/ConditionSystem.h"
#include "TimerSystem.h"
#include "UI/IntuitionGainedWidget.h"

IntuitionComponent::IntuitionComponent()
{
}

Properties IntuitionComponent::GetProps()
{
	Properties props("IntuitionComponent");
	props.Add("Intuition Name", &intuitionName);
	props.Add("Description", &intuitionDescription);
	props.Add("Condition", &condition);
	props.Add("Add On Interact", &addOnInteract);
	return props;
}

bool IntuitionComponent::CreateIntuition(std::string actorAquiredFromName)
{
	auto intuitionIt = GameInstance::playerIntuitions.find(intuitionName);
	if (intuitionIt != GameInstance::playerIntuitions.end())
	{
		Log("%s Intuition already known.", intuitionName.c_str());
		return false;
	}

	auto intuition = Intuition();
	intuition.name = intuitionName;
	intuition.description = intuitionDescription;

	intuition.actorAquiredFrom = actorAquiredFromName;
	intuition.worldAquiredFrom = world.worldFilename;

	intuition.hourAquired = GameInstance::currentHour;
	intuition.minuteAquired = GameInstance::currentMinute;

	//Check if intuition condition passes
	if (!condition.empty())
	{
		intuition.conditionFunc = conditionSystem.FindCondition(condition);
		intuition.conditionFuncName = condition;

		if (!intuition.conditionFunc())
		{
			return false; //intuition not created
		}
	}

	//Create intuiton
	GameInstance::playerIntuitions.emplace(intuition.name, intuition);
	Log("%s Intuition created.", intuition.name.c_str());

	auto intuitionGainedWidget = CreateWidget<IntuitionGainedWidget>();
	intuitionGainedWidget->intuitionToDisplay = &GameInstance::playerIntuitions[intuition.name];

	intuitionGainedWidget->AddToViewport();

	GameUtils::PlayAudio("purchase.wav");

	timerSystem.SetTimer(3.0f, std::bind(&IntuitionGainedWidget::DestroyAndRemove, intuitionGainedWidget));

	return true; //intuition created
}
