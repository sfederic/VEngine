#include "vpch.h"
#include "SequencerTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Editor/Sequencer/Sequencer.h"

SequencerTrigger::SequencerTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	SetRootComponent(boxTrigger);
}

void SequencerTrigger::Start()
{
	__super::Start();

	boxTrigger->SetTargetAsPlayer();
}

void SequencerTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (boxTrigger->ContainsTarget())
	{
		gSequencer.ActivateSequencer();
	}
}

Properties SequencerTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Sequencer File", &sequencerFile).autoCompletePath = "/SequencerFiles/";
	return props;
}
