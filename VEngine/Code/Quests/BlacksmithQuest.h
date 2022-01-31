#pragma once
#include "QuestLine.h"

struct BlacksmithQuest : QuestLine
{
	virtual bool Hour1() override;
};
