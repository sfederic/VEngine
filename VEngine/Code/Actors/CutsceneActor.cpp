#include "vpch.h"
#include "CutsceneActor.h"
#include "Components/BoxTriggerComponent.h"
#include "Editor/CutsceneSequencer.h"

CutsceneActor::CutsceneActor()
{
    boxTriggerComponent = BoxTriggerComponent::system.Add("BoxTrigger", this);
    boxTriggerComponent->renderWireframeColour = XMFLOAT4(0.3f, 0.2f, 0.75f, 1.f);
    rootComponent = boxTriggerComponent;
}

void CutsceneActor::Start()
{
    boxTriggerComponent->SetTargetAsPlayer();
}

void CutsceneActor::Tick(float deltaTime)
{
    if (boxTriggerComponent->ContainsTarget() && !cutscenePlayed)
    {
        PlayCutscene();
        cutscenePlayed = true;
    }
}

Properties CutsceneActor::GetProps()
{
    auto props = __super::GetProps();
    props.Add("Cutscene Filename", &cutsceneFilename);
    return props;
}

void CutsceneActor::PlayCutscene(bool loop)
{
    cutsceneSequencer.LoadCutscene(cutsceneFilename);
    cutsceneSequencer.StartPlayback(loop);
}
