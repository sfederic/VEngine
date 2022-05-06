#include "PostProcessInstance.h"
#include "Components/EmptyComponent.h"
#include "Render/RenderTypes.h"

PostProcessInstance::PostProcessInstance()
{
    rootComponent = EmptyComponent::system.Add(this);
}

Properties PostProcessInstance::GetProps()
{
    auto props = __super::GetProps();
    props.title = "PostProcessInstance";
    props.Add("Gamma", &gamma);
    props.Add("Exposure", &exposure);
    return props;
}

void PostProcessInstance::SetShaderPostProcessData(ShaderPostProcessData& data)
{
    data.gamma = this->gamma;
    data.exposure = this->exposure;
}
