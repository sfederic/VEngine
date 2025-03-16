#pragma once

#define COMPONENT_SYSTEM(type) \
inline static ComponentSystem<type> system; \
virtual void Remove() override { system.Remove(GetIndex()); } \
