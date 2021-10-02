#include "DialogueComponent.h"

void DialogueComponent::Tick(double deltaTime)
{
}

void DialogueComponent::Create()
{
    dialogue.LoadFromFile();
}

Properties DialogueComponent::GetProps()
{
    Properties props("Dialogue Component");
    props.Add("File", &dialogue.filename);
    return props;
}
