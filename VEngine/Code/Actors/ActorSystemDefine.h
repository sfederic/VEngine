#pragma once

#define ACTOR_SYSTEM(type) inline static ActorSystem<type> system; \
virtual void Remove() override { __super::Remove(); Destroy(); system.Remove(GetSystemIndex()); } \
