export module Actors.AnimCube;

import Actors.Actor;
import Actors.ActorSystemDefine;
import Core.Properties;

class SkeletalMeshComponent;
class SocketMeshComponent;

export class AnimCube : public Actor, public ActorSystemDefine<AnimCube>
{
public:
	AnimCube();
	void Create() override;
	void PostCreate() override;
	Properties GetProps() override;

private:
	SkeletalMeshComponent* skeletalMesh = nullptr;
	//SocketMeshComponent* socketMesh = nullptr;
};

AnimCube::AnimCube()
{
	skeletalMesh = CreateComponent<SkeletalMeshComponent>("Skeleton");
	SetRootComponent(skeletalMesh);
}

void AnimCube::Create()
{
	skeletalMesh->SetMeshFilename("cube_left_right.vmesh");
	skeletalMesh->SetTexture("test.png");
}

void AnimCube::PostCreate()
{
	skeletalMesh->LoadAnimations("cube_left_right.vanim");
}

Properties AnimCube::GetProps()
{
	return __super::GetProps();
}
