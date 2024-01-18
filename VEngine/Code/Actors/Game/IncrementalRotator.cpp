#include "vpch.h"
#include "IncrementalRotator.h"

void IncrementalRotator::Start()
{
	__super::Start();

	actorToRotate = World::GetActorByNameAndLogCast<GridActor>(actorNameToRotate);
	SetPlayerFocusGridActor(actorToRotate);
}

Properties IncrementalRotator::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Actor To Rotate", &actorNameToRotate);
	props.Add("Rotation Increment", &rotationIncrement);
	props.Add("Rotate Direction", &rotateDirection);
	return props;
}

void IncrementalRotator::OnLinkRotateLeft()
{
	__super::OnLinkRotateLeft();

	if (CheckActorToRotate())
	{
		XMVECTOR newNextRot = XMQuaternionMultiply(actorToRotate->GetNextRot(),
			DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&rotateDirection),
				XMConvertToRadians(rotationIncrement)));
		actorToRotate->SetNextRot(newNextRot);
	}
}

void IncrementalRotator::OnLinkRotateRight()
{
	__super::OnLinkRotateRight();

	if (CheckActorToRotate())
	{
		XMVECTOR newNextRot = XMQuaternionMultiply(actorToRotate->GetNextRot(),
			DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&rotateDirection),
				XMConvertToRadians(-rotationIncrement)));
		actorToRotate->SetNextRot(newNextRot);
	}
}

bool IncrementalRotator::CheckActorToRotate()
{
	if (actorToRotate == nullptr)
	{
		Log("No actor [%s] for [%s] to rotate.", actorNameToRotate.c_str(), GetName().c_str());
		return false;
	}
	return true;
}
