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
    auto quest = FindQuest(questName.c_str());

    if (!quest->isActive)
    {
        return false;
    }

    quest->ActivateOnHour();

    return false;
}

Quest* QuestComponent::GetQuest()
{
    auto quest = FindQuest(questName.c_str());
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
