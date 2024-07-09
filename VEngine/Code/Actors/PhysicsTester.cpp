#include "vpch.h"
#include "PhysicsTester.h"
#include "Physics/Raycast.h"
#include "Physics/PhysicsSystem.h"
#include "Core/Input.h"
#include "Core/Profile.h"

PhysicsTester::PhysicsTester()
{
	SetEmptyRootComponent();
}

void PhysicsTester::Tick(float deltaTime)
{
	if (Input::GetKeyUp(Keys::T))
	{
		const int iterations = 20000;
		{
			auto s = Profile::QuickStart();
			HitResult hit(this);

			for (int i = 0; i < iterations; i++)
			{
				PhysicsPhysx::Raycast(GetPosition(), GetForwardVector(), 10.f, hit);
			}
			auto e = Profile::QuickEnd(s);
			Log("Physx raycast: %f", e);
		}

		{
			auto s = Profile::QuickStart();
			HitResult hit(this);
			for (int i = 0; i < iterations; i++)
			{
				Physics::Raycast(hit, GetPositionV(), GetForwardVectorV(), 10.f);
			}
			auto e = Profile::QuickEnd(s);
			Log("Vengine raycast: %f", e);
		}
	}
}
