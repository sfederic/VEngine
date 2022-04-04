#include "CutsceneActor.h"
#include "Components/BoxTriggerComponent.h"

CutsceneActor::CutsceneActor()
{
    boxTriggerComponent = BoxTriggerComponent::system.Add(this);
    boxTriggerComponent->renderWireframeColour = XMFLOAT4(0.3f, 0.2f, 0.75f, 1.f);
    rootComponent = boxTriggerComponent;
}

Properties CutsceneActor::GetProps()
{
    auto props = __super::GetProps();
    props.AddProp(cutsceneFile);
    return props;
}

void CutsceneActor::PlayCutscene()
{
    //@Todo: need to consolidate CutsceneSequencer and CutsceneActor.
    //Make CutsceneSequencer a global and run everything here related to playing through that.
}
