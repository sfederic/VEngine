#include "vpch.h"
#include "Player.h"
#include <limits>
#include <array>
#include "Input.h"
#include "VMath.h"
#include "Actors/Game/NoteActor.h"
#include "Actors/Game/InteractActor.h"
#include "Components/CameraComponent.h"
#include "Components/EmptyComponent.h"
#include "Components/Game/PhotoComponent.h"
#include "Physics/Raycast.h"
#include "Render/Renderer.h"
#include "UI/Game/ScanWidget.h"
#include "UI/Game/PhotoWidget.h"
#include "UI/Game/SalvageMissionWidget.h"
#include "UI/Game/DialogueWidget.h"
#include "UI/Game/PlayerActionBarWidget.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/DialogueStructures.h"

const int movementIncrement = 1;

int photoFilenameIndex = 0;
std::array<std::wstring, 5> photoFilenames;

Dialogue dialogue;
int dialogueCurrentLine = 0;

Player::Player()
{
	rootComponent = CreateComponent(EmptyComponent(), "Root");

	camera = CreateComponent(CameraComponent(), "Camera");
	rootComponent->AddChild(camera);

	photoFilenames[0] = L"photo0.jpg";
	photoFilenames[1] = L"photo1.jpg";
	photoFilenames[2] = L"photo2.jpg";
	photoFilenames[3] = L"photo3.jpg";
	photoFilenames[4] = L"photo4.jpg";
}

void Player::Start()
{
	CreatePlayerWidgets();

	nextPos = GetPositionV();
	nextRot = GetRotationV();
}

void Player::Tick(float deltaTime)
{
	camera->upViewVector = GetUpVectorV();

	if (inCombat)
	{
		actionBarWidget->actionPoints = combatActionPoints;
	}

	SetMovementAxis();
    MovementInput(deltaTime);

	ShootInput();
	Interact();

	ProgressDialogue();

	ScanVisorInputToggle();
	Scan();

	ToggleSalvageMissionStats();

	TakePhoto();

	SpawnNote();

	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotSpeed));
}

Properties Player::GetProps()
{
    return __super::GetProps();
}

//@Todo: move all dialogue code somewhere else
void Player::StartDialogue(std::string dialogueFilename)
{
	dialogueWidget->AddToViewport();

	dialogue.Reset();
	dialogue.filename = dialogueFilename;
	dialogue.LoadFromFile();

	auto foundLineIt = dialogue.data.find(dialogueCurrentLine);
	if (foundLineIt != dialogue.data.end())
	{
		dialogueWidget->dialogueText = foundLineIt->second.text;

		dialogueCurrentLine++;
	}
	else
	{
		EndDialogue();
	}
}

void Player::SetInCombat(bool combatActive)
{
	inCombat = combatActive;

	actionBarWidget->actionPoints = combatActionPoints;
	actionBarWidget->AddToViewport();
}

void Player::ProgressDialogue()
{
	if (Input::GetKeyDown(Keys::Down))
	{
		auto foundLineIt = dialogue.data.find(dialogueCurrentLine);
		if (foundLineIt != dialogue.data.end())
		{
			dialogueWidget->dialogueText = foundLineIt->second.text;

			dialogueCurrentLine++;
		}
		else
		{
			EndDialogue();
		}
	}
}

void Player::EndDialogue()
{
	Log("Dialouge [%s] ended", dialogue.filename.c_str());
	dialogueWidget->RemoveFromViewport();
	dialogueCurrentLine = 0;
	dialogue.Reset();
}

bool Player::CombatMoveCheck()
{
	if (inCombat)
	{
		if (combatActionPoints > 0)
		{
			combatActionPoints--;
			return true;
		}

		Log("Cannot move. Not enough action points during combat.");
		return false;
	}

	return true;
}

void Player::MovementInput(float deltaTime)
{
	if (CheckIfPlayerMovementAndRotationStopped())
	{
		if (shakeOnWallRotateEnd)
		{
			GameUtils::CameraShake(0.5f);
			shakeOnWallRotateEnd = false;
		}

		XMVECTOR previousPos = GetPositionV();

		if (Input::GetKeyHeld(Keys::W))
		{
			if (!RotatePlayerOnWallMoveHit(rightAxis, forwardAxis, -90.f, -90.f, 90.f, 90.f))
			{
				if (CombatMoveCheck())
				{
					nextPos = GetPositionV() + (forwardAxis * movementIncrement);

					if (IsFloorEmptyOnNextMove())
					{
						nextPos = previousPos;
					}
				}
			}
		}
		else if (Input::GetKeyHeld(Keys::S))
		{
			if (!RotatePlayerOnWallMoveHit(rightAxis, -forwardAxis, 90.f, 90.f, -90.f, -90.f))
			{
				if (CombatMoveCheck())
				{
					nextPos = GetPositionV() - (forwardAxis * movementIncrement);

					if (IsFloorEmptyOnNextMove())
					{
						nextPos = previousPos;
					}
				}
			}
		}
		else if (Input::GetKeyHeld(Keys::A))
		{
			if (!RotatePlayerOnWallMoveHit(forwardAxis, -rightAxis, -90.f, -90.f, 90.f, 90.f))
			{
				if (CombatMoveCheck())
				{
					nextPos = GetPositionV() - (rightAxis * movementIncrement);

					if (IsFloorEmptyOnNextMove())
					{
						nextPos = previousPos;
					}
				}
			}
		}
		else if (Input::GetKeyHeld(Keys::D))
		{
			if (!RotatePlayerOnWallMoveHit(forwardAxis, rightAxis, 90.f, 90.f, -90.f, -90.f))
			{
				if (CombatMoveCheck())
				{
					nextPos = GetPositionV() + (rightAxis * movementIncrement);

					if (IsFloorEmptyOnNextMove())
					{
						nextPos = previousPos;
					}
				}
			}
		}
	}
}

bool Player::CheckIfPlayerMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}

void Player::SetMovementAxis()
{
	movementAxes[0] = GetForwardVectorV();
	movementAxes[1] = -GetForwardVectorV();
	movementAxes[2] = GetRightVectorV();
	movementAxes[3] = -GetRightVectorV();

	float forwardAngle = std::numeric_limits<float>::max();
	float rightAngle = std::numeric_limits<float>::max();

	for (int i = 0; i < 4; i++)
	{
		const float tempForwardAngle =
			std::acos(XMVector3Dot(camera->GetForwardVectorV(), movementAxes[i]).m128_f32[0]);
		if (tempForwardAngle < forwardAngle)
		{
			forwardAngle = tempForwardAngle;
			forwardAxis = movementAxes[i];
		}

		const float tempRightAngle =
			std::acos(XMVector3Dot(camera->GetRightVectorV(), movementAxes[i]).m128_f32[0]);
		if (tempRightAngle < rightAngle)
		{
			rightAngle = tempRightAngle;
			rightAxis = movementAxes[i];
		}
	}
}

bool Player::RotatePlayerOnWallMoveHit(XMVECTOR movementAxis, XMVECTOR raycastAxis,
	float rightAngle, float forwardAngle, float leftAngle, float backAngle)
{
	Ray ray(this);
	if (Raycast(ray, GetPositionV(), GetPositionV() + (raycastAxis * movementIncrement)))
	{
		if (XMQuaternionEqual(movementAxis, GetRightVectorV()))
		{
			nextRot = XMQuaternionMultiply(
				nextRot, XMQuaternionRotationAxis(VMath::GlobalRightVector(), XMConvertToRadians(rightAngle)));
		}
		else if (XMQuaternionEqual(movementAxis, GetForwardVectorV()))
		{
			nextRot = XMQuaternionMultiply(
				nextRot, XMQuaternionRotationAxis(VMath::GlobalForwardVector(), XMConvertToRadians(forwardAngle)));
		}
		else if (XMQuaternionEqual(movementAxis, -GetRightVectorV()))
		{
			nextRot = XMQuaternionMultiply(
				nextRot, XMQuaternionRotationAxis(VMath::GlobalRightVector(), XMConvertToRadians(leftAngle)));
		}
		else if (XMQuaternionEqual(movementAxis, -GetForwardVectorV()))
		{
			nextRot = XMQuaternionMultiply(
				nextRot, XMQuaternionRotationAxis(VMath::GlobalForwardVector(), XMConvertToRadians(backAngle)));
		}

		shakeOnWallRotateEnd = true;

		return true;
	}

	return false;
}

void Player::ShootInput()
{
	if (Input::GetMouseLeftUp())
	{
		Ray ray(this);
		const float shootDistance = 50.f;
		if (Raycast(ray, GetPositionV(), GetPositionV() + (camera->GetForwardVectorV() * shootDistance)))
		{
			ray.hitActor->Destroy();
		}
	}
}

void Player::Interact()
{
	if (Input::GetMouseRightUp())
	{
		Ray ray(this);
		const float interactDistance = 2.0f;
		if (Raycast(ray, GetPositionV(), GetPositionV() + (camera->GetForwardVectorV() * interactDistance)))
		{
			InteractActor* interactActor = dynamic_cast<InteractActor*>(ray.hitActor);
			if (interactActor)
			{
				interactActor->Interact();
			}
		}
	}
}

bool Player::IsFloorEmptyOnNextMove()
{
	XMVECTOR origin = nextPos;
	XMVECTOR up = GetUpVectorV();
	XMVECTOR end = origin - (up * movementIncrement);

	Ray ray(this);
	if (!Raycast(ray, origin, end))
	{
		Log("Cannot move to empty spot.");
		return true;
	}

	return false;
}

void Player::Scan()
{
	if (!scanVisorActive) return;

	XMVECTOR origin = GetPositionV();
	const float scanRange = 50.f;
	XMVECTOR end = origin + (camera->GetForwardVectorV() * scanRange);

	Ray ray(this);
	if (Raycast(ray, origin, end))
	{
		Actor* scanTarget = ray.hitActor;
		if (scanTarget)
		{
			scanWidget->SetScanInfoText(scanTarget->scanText);
		}
		else
		{
			scanWidget->ResetValues();
		}
	}
	else
	{
		scanWidget->ResetValues();
	}
}

void Player::TakePhoto()
{
	if (Input::GetKeyDown(Keys::Num3))
	{
		if (photoFilenameIndex < photoFilenames.size())
		{
			Renderer::PlayerPhotoCapture(photoFilenames.at(photoFilenameIndex));

			RaycastAgainstActorWithPhotoComponent();

			photoWidget->photoFilename = VString::wstos(photoFilenames.at(photoFilenameIndex));
			photoWidget->AddToViewport(3.f);

			photoFilenameIndex++;
		}
		else
		{
			Log("Out of film for photos.");
		}
	}
}

void Player::RaycastAgainstActorWithPhotoComponent()
{
	Ray ray(this);
	const float photoDistance = 100.f;
	if (Raycast(ray, GetPositionV(), GetPositionV() + (camera->GetForwardVectorV() * photoDistance)))
	{
		PhotoComponent* photoComponent = ray.hitActor->GetFirstComponentOfTypeAllowNull<PhotoComponent>();
		if (photoComponent)
		{
			if (photoComponent->IsTagPartOfCurrentSalvage())
			{
				std::string photoTag = photoComponent->GetPhotoTag();
				GameInstance::playerPhotoTagsCaptured.insert(photoTag);
				Log("Photo with tag [%s] taken", photoTag.c_str());
			}
		}
	}
}

void Player::ScanVisorInputToggle()
{
	if (Input::GetKeyDown(Keys::Num1))
	{
		scanVisorActive = !scanVisorActive;

		if (scanVisorActive)
		{
			scanWidget->AddToViewport();
		}
		else
		{
			scanWidget->RemoveFromViewport();
		}
	}
}

void Player::CreatePlayerWidgets()
{
	scanWidget = CreateWidget<ScanWidget>();
	photoWidget = CreateWidget<PhotoWidget>();
	salvageMissionWidget = CreateWidget<SalvageMissionWidget>();
	dialogueWidget = CreateWidget<DialogueWidget>();
	actionBarWidget = CreateWidget<PlayerActionBarWidget>();
}

void Player::SpawnNote()
{
	if (Input::GetMouseRightUp())
	{
		//@Todo: spawn on raycast hit
		NoteActor* noteActor = NoteActor::system.Add(NoteActor(), GetTransform());
		noteActor->SetNoteText(L"Testing note text");
		noteActor->AddNoteWidgetToViewport();
	}
}

void Player::ToggleSalvageMissionStats()
{
	if (Input::GetKeyDown(Keys::Enter))
	{
		salvageMissionMenuOpen = !salvageMissionMenuOpen;

		if (salvageMissionMenuOpen)
		{
			salvageMissionWidget->AddToViewport();
		}
		else
		{
			salvageMissionWidget->RemoveFromViewport();
		}
	}
}
