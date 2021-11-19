#include "DialogueInstance.h"
#include "Components/DialogueComponent.h"
#include "Components/EmptyComponent.h"

DialogueInstance::DialogueInstance()
{
    dialogueComponent = DialogueComponent::system.Add(this);
    rootComponent = EmptyComponent::system.Add(this);
}

Properties DialogueInstance::GetProps()
{
    return Properties("DialogueInstance");
}
