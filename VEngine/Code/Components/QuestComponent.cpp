#include "QuestComponent.h"
#include "Quests/QuestSystem.h"
#include "Quests/Quest.h"

Properties QuestComponent::GetProps()
{
    Properties props("QuestComponent");
    props.Add("Quest Name", &questName);
    return props;
}

bool QuestComponent::CheckQuest()
{
    auto quest = FindQuest(questName.c_str());

    if (!quest->isActive)
    {
        return false;
    }

    quest->ActivateOnHour();

    return false;
}
