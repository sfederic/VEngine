#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct CameraComponent;
struct MeshComponent;

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
