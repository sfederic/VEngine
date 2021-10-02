#include "DialogueComponent.h"

void DialogueComponent::Tick(double deltaTime)
{
}

void DialogueComponent::Start()
{
    dialogue.LoadFromFile();
}

void DialogueComponent::Create()
{
}

Properties DialogueComponent::GetProps()
{
    Properties props("Dialogue Component");
    props.Add("File", &dialogue.filename);
    return props;
}
