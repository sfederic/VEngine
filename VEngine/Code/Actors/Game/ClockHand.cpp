#include "vpch.h"
#include "ClockHand.h"

Properties ClockHand::GetProps()
{
    auto props = __super::GetProps();
    props.title = GetTypeName();
    return props;
}
