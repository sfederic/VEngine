#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct CameraComponent;
struct MeshComponent;

//Engine really should only be working with one ReflectionPlane per level. Asserts in rederer code for that.
//@Todo: planar reflection code is junk. See if it's worth fixing up.
struct ReflectionPlane : Actor
{
	ACTOR_SYSTEM(ReflectionPlane);

	CameraComponent* reflectionCamera = nullptr;

	MeshComponent* planeMesh = nullptr;

	XMFLOAT3 reflectDirection;

	ReflectionPlane();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	XMMATRIX GetReflectionViewMatrix();
};
