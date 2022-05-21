#include "vpch.h"
#include "QuestComponent.h"
#include "Quests/QuestSystem.h"
#include "Quests/Quest.h"
#include "Log.h"

Properties QuestComponent::GetProps()
{
    Properties props("QuestComponent");
    props.Add("Quest Name", &questName);
    return props;
}

bool QuestComponent::CheckQuest()
{
    auto quest = questSystem.FindQuest(questName.c_str());

    if (!quest->isActive)
    {
        return false;
    }

    quest->ActivateOnHour();

    return false;
}

Quest* QuestComponent::GetQuest()
{
    auto quest = questSystem.FindQuest(questName.c_str());
    if (quest)
    {
        return quest;
    }

    return nullptr;
}

void QuestComponent::DeactivateQuest()
{
    auto quest = GetQuest();
    if (quest)
    {
        quest->isActive = false;
        Log("Quest [%s] deactivated.", questName.c_str());
    }
}

bool QuestComponent::CheckIfQuestActive()
{
    auto quest = GetQuest();
    if (quest) //Not every actor will have a quest set to it
    {
        return quest->isActive;
    }

    return true;
}
