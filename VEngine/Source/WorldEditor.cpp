#include "WorldEditor.h"
#include "Input.h"
#include "Camera.h"
#include "UISystem.h"
#include "World.h"
#include "CoreSystem.h"
#include "ShaderFactory.h"
#include "MathHelpers.h"
#include "..\EditorMainWindow.h"
#include "PropertiesDock.h"
#include "EditorSystem.h"
#include "Actors/TestActor.h"
#include "ActorSystemFactory.h"

WorldEditor gWorldEditor;

void WorldEditor::Tick(ID3D11Buffer* debugLinesBuffer)
{
	if (gUISystem.bUIClicked)
	{
		return;
	}

	//Turn off currently selected actor spawn systsm.
	if (gInputSystem.GetKeyUpState(Keys::BackSpace))
	{
		ActorSystemFactory::SetCurrentActiveActorSystem(nullptr);
	}

	World* world = GetWorld();

	if (pickedActor)
	{
		//Duplicate selected actor
		if (gInputSystem.GetAsyncKey(Keys::Ctrl))
		{
			if (gInputSystem.GetKeyDownState(Keys::D))
			{
				Transform transform = pickedActor->transform;
				pickedActor->linkedActorSystem->SpawnActor(transform);

				//Refresh world dock
				gEditorSystem->PopulateWorldList();
			}
		}

		//Transform gizmo on button presses (i.e using arrows for rotation etc.)

		//ROTATION
		if (gInputSystem.GetKeyUpState('R'))
		{
			bRotateActors = !bRotateActors;
			bTranslateActors = false;
			bScaleActors = false;
		}

		if (bRotateActors)
		{
			if (gInputSystem.GetAsyncKey(Keys::Right))
			{
				rotateSpeed += 1.0f * gCoreSystem.deltaTime;
				pickedActor->SetRotation(XMVectorUp(), rotateSpeed);
			}
			else
			{
				rotateSpeed = 0.f;
			}
		}

		//SCALE
		if (gInputSystem.GetKeyUpState('E'))
		{
			bScaleActors = ! bScaleActors;
			bRotateActors = false;
			bTranslateActors = false;
		}

		if (bScaleActors)
		{
			const float deltaTime = gCoreSystem.deltaTime;

			if (gInputSystem.GetAsyncKey(Keys::Up))
			{
				XMFLOAT3 scale = pickedActor->GetScale();
				scale.x += scaleSpeed * deltaTime;
				scale.y += scaleSpeed * deltaTime;
				scale.z += scaleSpeed * deltaTime;
				pickedActor->SetScale(scale);
			}
			else if (gInputSystem.GetAsyncKey(Keys::Down))
			{
				XMFLOAT3 scale = pickedActor->GetScale();
				if (scale.x > 0.f)
				{
					scale.x -= scaleSpeed * deltaTime;
					scale.y -= scaleSpeed * deltaTime;
					scale.z -= scaleSpeed * deltaTime;
					pickedActor->SetScale(scale);
				}
			}
		}

		//TRANSLATION
		/*if (gInputSystem.GetKeyUpState('W'));
		{
			bTranslateActors = !bTranslateActors;
			bScaleActors = false;
			bRotateActors = false;
		}*/

		if (bTranslateActors)
		{
			if (gInputSystem.GetAsyncKey(Keys::Ctrl))
			{
				if (gInputSystem.GetKeyDownState(VK_UP))
				{
					pickedActor->Move(moveIncrement, XMVectorUp());
				}
				else if (gInputSystem.GetKeyDownState(VK_DOWN))
				{
					pickedActor->Move(-moveIncrement, XMVectorUp());
				}
			}
			else if (gInputSystem.GetKeyDownState(VK_UP))
			{
				pickedActor->Move(moveIncrement, XMVectorForward());
			}
			else if (gInputSystem.GetKeyDownState(VK_DOWN))
			{
				pickedActor->Move(-moveIncrement, XMVectorForward());
			}
			else if (gInputSystem.GetKeyDownState(VK_LEFT))
			{
				pickedActor->Move(-moveIncrement, XMVectorRight());
			}
			else if (gInputSystem.GetKeyDownState(VK_RIGHT))
			{
				pickedActor->Move(moveIncrement, XMVectorRight());
			}
		}

		//Set properties widget info
		{
			/*PropertiesWidget* propWidget = editorMainWindow->propertiesDock->propWidget;

			XMFLOAT3 pos = pickedActor->GetPositionFloat3();
			propWidget->posEditX->setText(QString::number(pos.x));
			propWidget->posEditY->setText(QString::number(pos.y));
			propWidget->posEditZ->setText(QString::number(pos.z));

			XMFLOAT3 scale = pickedActor->GetScale();
			propWidget->scaleEditX->setText(QString::number(scale.x));
			propWidget->scaleEditY->setText(QString::number(scale.y));
			propWidget->scaleEditZ->setText(QString::number(scale.z));

			XMFLOAT3 rot = pickedActor->GetRollPitchYaw();
			propWidget->rotEditX->setText(QString::number(rot.x));
			propWidget->rotEditY->setText(QString::number(rot.y));
			propWidget->rotEditZ->setText(QString::number(rot.z));*/
		}

		//Delete actors in editor
		if (gInputSystem.GetKeyUpState(Keys::Delete))
		{
			world->GetActorSystem(screenPickRay.actorSystemIndex)->RemoveActor(screenPickRay.actorIndex);
			pickedActor = nullptr;

			gEditorSystem->PopulateWorldList();
		}
	}


	//Actor picking for editor
	if (gInputSystem.GetMouseLeftDownState() && !gUISystem.bUIClicked)
	{
		lastMousePosX = gUISystem.mousePos.x;
		lastMousePosY = gUISystem.mousePos.y;

		if (RaycastAllFromScreen(screenPickRay, gUISystem.mousePos.x, gUISystem.mousePos.y, &editorCamera, GetWorld()))
		{
			//Actor spawning based on current picked system in world dock
			if (ActorSystemFactory::GetCurrentActiveActorSystem())
			{
				ActorSystem* actorSystemToSpawnFrom = ActorSystemFactory::GetCurrentActiveActorSystem();
				Transform transform = Transform();
				transform.position = screenPickRay.hitPos;
				actorSystemToSpawnFrom->SpawnActor(transform);

				GetWorld()->AddActorSystem(actorSystemToSpawnFrom);
			}

			if (RaycastTriangleIntersect(screenPickRay))
			{
				pickedDirection = XMLoadFloat3(&screenPickRay.normal);
				DebugPrint("%f %f %f\n", screenPickRay.normal.x, screenPickRay.normal.y, screenPickRay.normal.z);
			}

			if (debugLinesBuffer)
			{
				DrawRayDebug(screenPickRay.origin, screenPickRay.direction, screenPickRay.distance, debugLinesBuffer);
			}

			actorIndex = screenPickRay.actorIndex;
			actorSystemIndex = screenPickRay.actorSystemIndex;

			if (pickedActor)
			{
				//Properties Dock Panel
				{
					ActorSystem* actorSystemToDisplay = GetWorld()->GetActorSystem(actorSystemIndex);
					gEditorSystem->DisplayActorSystemProperties(pickedActor);
				}

				pickedActor->bPicked = true;
			}
		}
		else
		{
			if (pickedActor)
			{
				pickedActor->bPicked = false;
			}
		}
	}
}

void WorldEditor::Init()
{

}

void WorldEditor::MoveActor(Actor* actor, XMVECTOR direction)
{
	int mouseX = gUISystem.mousePos.x;
	int mouseY = gUISystem.mousePos.y;

	if (gInputSystem.GetAsyncKey(Keys::LeftMouse))
	{
		SetCapture(gCoreSystem.mainWindow);

		float dx = XMConvertToRadians(0.25f * (float)(mouseX - lastMousePosX));
		float dy = XMConvertToRadians(0.25f * (float)(mouseY - lastMousePosY));

		if (actor)
		{
			XMVECTOR up = XMVectorEqual(direction, XMVectorUp());
			//if (up.m128_f32[0] == 0)
			{
				actor->Move((dy + dx) * pickedActorMoveSpeed, direction);
			}
		}

		ReleaseCapture();
	}

	lastMousePosX = mouseX;
	lastMousePosY = mouseY;
}

//Actors as the movement along the translation axis using mouse (Different from Actor::Move())
void WorldEditor::MoveActor(Actor* actor)
{
	int mouseX = gUISystem.mousePos.x;
	int mouseY = gUISystem.mousePos.y;

	if (gInputSystem.GetAsyncKey(Keys::LeftMouse))
	{
		SetCapture(gCoreSystem.mainWindow);

		float dx = XMConvertToRadians(0.25f * (float)(mouseX - lastMousePosX));
		float dy = XMConvertToRadians(0.25f * (float)(mouseY - lastMousePosY));

		ReleaseCapture();
	}

	lastMousePosX = mouseX;
	lastMousePosY = mouseY;
}
