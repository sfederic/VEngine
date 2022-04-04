#include "CutsceneActor.h"
#include "Components/BoxTriggerComponent.h"
#include "Editor/CutsceneSequencer.h"

CutsceneActor::CutsceneActor()
{
    boxTriggerComponent = BoxTriggerComponent::system.Add(this);
    boxTriggerComponent->renderWireframeColour = XMFLOAT4(0.3f, 0.2f, 0.75f, 1.f);
    rootComponent = boxTriggerComponent;
}

Properties CutsceneActor::GetProps()
{
    auto props = __super::GetProps();
    props.AddProp(cutsceneFilename);
    return props;
}

void CutsceneActor::PlayCutscene(bool loop = false)
{
    cutsceneSequencer.LoadCutscene(cutsceneFilename);
    cutsceneSequencer.StartPlayback(loop);
}
