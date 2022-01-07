#include "IntuitionComponent.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Log.h"
#include "VString.h"
#include "World.h"
#include "Gameplay/ConditionSystem.h"
#include "Timer.h"
#include "UI/IntuitionGainedWidget.h"
#include "UI/UISystem.h"

IntuitionComponent::IntuitionComponent()
{
}

Properties IntuitionComponent::GetProps()
{
	Properties props("IntuitionComponent");
	props.Add("Intuition Name", &intuitionName);
	props.Add("Description", &intuitionDescription);
	props.Add("Condition", &condition);
	props.Add("Condition Value", &conditionArg);
	props.Add("Add On Interact", &addOnInteract);
	return props;
}

bool IntuitionComponent::CreateIntuition(std::string actorAquiredFromName)
{
	if (intuitionName.empty())
	{
		Log("%s Intuition name not set.", actorAquiredFromName.c_str());
		return false;
	}

	auto intuitionIt = GameInstance::playerIntuitions.find(VString::wstos(intuitionName));
	if (intuitionIt != GameInstance::playerIntuitions.end())
	{
		Log("%s Intuition already known.", intuitionName.c_str());
		return false;
	}

	auto intuition = Intuition();
	intuition.name = VString::wstos(intuitionName);
	intuition.description = VString::wstos(intuitionDescription);

	intuition.actorAquiredFrom = actorAquiredFromName;
	intuition.worldAquiredFrom = world.worldFilename;

	intuition.hourAquired = GameInstance::currentHour;
	intuition.minuteAquired = GameInstance::currentMinute;

	//Check if intuition condition passes
	if (!condition.empty())
	{
		intuition.conditionFunc = conditionSystem.FindCondition(condition);
		intuition.conditionFuncName = condition;

		if (!intuition.conditionFunc(conditionArg))
		{
			return false; //intuition not created
		}
	}

	//Create intuiton
	GameInstance::playerIntuitions.emplace(intuition.name, intuition);
	Log("%s Intuition created.", intuition.name.c_str());

	uiSystem.intuitionGainedWidget->intuitionToDisplay = &GameInstance::playerIntuitions[intuition.name];
	uiSystem.intuitionWidgetInViewport = true;
	uiSystem.intuitionGainedWidget->AddToViewport();

	GameUtils::PlayAudioOneShot("purchase.wav");

	return true; //intuition created
}
