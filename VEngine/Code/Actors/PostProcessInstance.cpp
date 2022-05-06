#include "PostProcessInstance.h"
#include "Components/EmptyComponent.h"

PostProcessInstance::PostProcessInstance()
{
    rootComponent = EmptyComponent::system.Add(this);
}

Properties PostProcessInstance::GetProps()
{
    auto props = __super::GetProps();
    props.Add("Gamma", &gamma);
    return props;
}
