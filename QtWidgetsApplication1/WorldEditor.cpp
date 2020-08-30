#include "WorldEditor.h"
#include "Input.h"
#include "Camera.h"
#include "UISystem.h"
#include "World.h"
#include "CoreSystem.h"
#include "ShaderFactory.h"
#include "MathHelpers.h"

WorldEditor gWorldEditor;

void WorldEditor::Tick(ID3D11Buffer* debugLinesBuffer)
{
	if (gUISystem.bUIClicked)
	{
		return;
	}

	World* world = GetWorld();

	if (pickedActor)
	{
		//xAxis.actors[0].SetPosition(world->GetActor(actorSystemIndex, actorIndex)->GetPositionVector());
		//yAxis.actors[0].SetPosition(world->GetActor(actorSystemIndex, actorIndex)->GetPositionVector());
		//zAxis.actors[0].SetPosition(world->GetActor(actorSystemIndex, actorIndex)->GetPositionVector());
		xAxis.actors[0].SetPosition(pickedActor->GetPositionVector());
		yAxis.actors[0].SetPosition(pickedActor->GetPositionVector());
		zAxis.actors[0].SetPosition(pickedActor->GetPositionVector());

		//MoveActor(pickedActor, pickedDirection);

		//TEST SPAWNING
		if (gInputSystem.GetKeyDownState(VK_SPACE))
		{
			//XMVECTOR spawnPos = world->GetActor(screenPickRay.actorSystemIndex, screenPickRay.actorIndex)->GetPositionVector() +
			//	pickedDirection * 2.f;

			XMVECTOR spawnPos = pickedActor->GetPositionVector() + (pickedDirection * 2.f);

			Actor* actor = world->GetActorSystem(0)->AddActor(spawnPos);
			pickedActor = actor;
		}

		if (gInputSystem.GetKeyUpState(VK_DELETE))
		{
			world->GetActorSystem(screenPickRay.actorSystemIndex)->RemoveActor(screenPickRay.actorIndex);
		}
	}


	if (pickedAxis)
	{
		if (pickedAxis->pickedAxis == PickedAxis::X)
		{
			MoveActor(pickedActor, PickedAxis::X);
		}
		else if (pickedAxis->pickedAxis == PickedAxis::Y)
		{
			MoveActor(pickedActor, PickedAxis::Y);
		}	
		else if (pickedAxis->pickedAxis == PickedAxis::Z)
		{
			MoveActor(pickedActor, PickedAxis::Z);
		}
	}

	//TODO: maybe make a camera axis system like MechaCrawler for arrow movement
	//Actor Arros key movement (For grid movement)
	if (pickedActor)
	{
		if (gInputSystem.GetAsyncKey(VK_CONTROL))
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
	

	//Actor picking for editor
	if (gInputSystem.GetMouseLeftUpState())
	{
		pickedAxis = nullptr;
	}
	else if (gInputSystem.GetMouseLeftDownState() && !gUISystem.bUIClicked)
	{
		lastMousePosX = gUISystem.mousePos.x;
		lastMousePosY = gUISystem.mousePos.y;

		if (RaycastAllFromScreen(screenPickRay, gUISystem.mousePos.x, gUISystem.mousePos.y, &editorCamera, GetWorld()))
		{
			if (RaycastTriangleIntersect(screenPickRay))
			{
				pickedDirection = XMLoadFloat3(&screenPickRay.normal);
				DebugPrint("%f %f %f\n", screenPickRay.normal.x, screenPickRay.normal.y, screenPickRay.normal.z);
			}

			if (debugLinesBuffer)
			{
				DrawRayDebug(screenPickRay.origin, screenPickRay.direction, screenPickRay.distance, debugLinesBuffer);
			}

			xAxis.actors[0].bRender = true;
			yAxis.actors[0].bRender = true;
			zAxis.actors[0].bRender = true;

			actorIndex = screenPickRay.actorIndex;
			actorSystemIndex = screenPickRay.actorSystemIndex;

			if (pickedActor)
			{
				pickedActor->bPicked = true;
			}
		}
		else
		{
			if (pickedActor)
			{
				pickedActor->bPicked = false;
				pickedActor = nullptr;
				
				xAxis.actors[0].bRender = false;
				yAxis.actors[0].bRender = false;
				zAxis.actors[0].bRender = false;
			}
		}
	}

	//TODO: this is no good. If the rendering code ever changes it needs to be mirrored here
	//Render translation widget
	for (int i = 0; i < axes.size(); i++)
	{
		ActorSystem* actorSystem = axes[i];

		//Set rastState
		if (actorSystem->rastState)
		{
			gRenderSystem.context->RSSetState(actorSystem->rastState);
		}
		else
		{
			gRenderSystem.context->RSSetState(gRenderSystem.activeRastState);
		}

		auto shader = gShaderFactory.shaderMap.find(actorSystem->shaderName);

		if (shader == gShaderFactory.shaderMap.end())
		{
			DebugPrint("vertex shader file name %ls not found\n", actorSystem->shaderName);
		}

		gRenderSystem.context->VSSetShader(shader->second->vertexShader, nullptr, 0);
		gRenderSystem.context->PSSetShader(shader->second->pixelShader, nullptr, 0);

		gRenderSystem.context->PSSetSamplers(0, 1, &actorSystem->samplerState);
		gRenderSystem.context->PSSetShaderResources(0, 1, &actorSystem->srv);

		UINT strides = sizeof(Vertex);
		UINT offsets = 0;

		gRenderSystem.context->IASetVertexBuffers(0, 1, &actorSystem->vertexBuffer, &strides, &offsets);
		gRenderSystem.context->IASetIndexBuffer(actorSystem->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

		for (int i = 0; i < actorSystem->actors.size(); i++)
		{
			if (actorSystem->actors[i].bRender)
			{
				gRenderSystem.matrices.view = GetPlayerCamera()->view;
				gRenderSystem.matrices.model = actorSystem->actors[i].transform;
				gRenderSystem.matrices.mvp = gRenderSystem.matrices.model * gRenderSystem.matrices.view * gRenderSystem.matrices.proj;
				gRenderSystem.context->UpdateSubresource(gRenderSystem.cbMatrices, 0, nullptr, &gRenderSystem.matrices, 0, 0);
				gRenderSystem.context->VSSetConstantBuffers(0, 1, &gRenderSystem.cbMatrices);

				gRenderSystem.context->Draw(actorSystem->modelData.verts.size(), 0);
				//context->DrawIndexed(actorSystem->modelData.indices.size(), 0, 0);
			}
		}
	}
}

void WorldEditor::Init()
{
	xAxis.modelName = "x_axis.fbx";
	yAxis.modelName = "y_axis.fbx";
	zAxis.modelName = "z_axis.fbx";

	xAxis.shaderName = L"depthDrawRed.hlsl";
	yAxis.shaderName = L"depthDrawBlue.hlsl";
	zAxis.shaderName = L"depthDrawGreen.hlsl";

	xAxis.CreateActors(&gRenderSystem, 1);
	yAxis.CreateActors(&gRenderSystem, 1);
	zAxis.CreateActors(&gRenderSystem, 1);

	xAxis.rastState = gRenderSystem.rastStateNoBackCull;
	yAxis.rastState = gRenderSystem.rastStateNoBackCull;
	zAxis.rastState = gRenderSystem.rastStateNoBackCull;

	xAxis.actors[0].bRender = false;
	yAxis.actors[0].bRender = false;
	zAxis.actors[0].bRender = false;

	xAxis.actors[0].pickedAxis = PickedAxis::X;
	yAxis.actors[0].pickedAxis = PickedAxis::Y;
	zAxis.actors[0].pickedAxis = PickedAxis::Z;

	axes.push_back(&xAxis);
	axes.push_back(&yAxis);
	axes.push_back(&zAxis);
}

void WorldEditor::MoveActor(Actor* actor, XMVECTOR direction)
{
	int mouseX = gUISystem.mousePos.x;
	int mouseY = gUISystem.mousePos.y;

	if (gInputSystem.GetAsyncKey(VK_LBUTTON))
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
void WorldEditor::MoveActor(Actor* actor, PickedAxis axis)
{
	int mouseX = gUISystem.mousePos.x;
	int mouseY = gUISystem.mousePos.y;

	if (gInputSystem.GetAsyncKey(VK_LBUTTON))
	{
		SetCapture(gCoreSystem.mainWindow);

		float dx = XMConvertToRadians(0.25f * (float)(mouseX - lastMousePosX));
		float dy = XMConvertToRadians(0.25f * (float)(mouseY - lastMousePosY));

		if (bMoveActorsInIncrements && actor)
		{
			//For incremental movement
			if (pickedAxis->pickedAxis == PickedAxis::X)
			{
				dxAccum += dx;
				if (dxAccum > 0.5f)
				{
					actor->Move(moveIncrement, XMVectorSet(1.f, 0.f, 0.f, 0.f));
					dxAccum = 0;
				}
				else if (dxAccum < -0.5f)
				{
					actor->Move(-moveIncrement, XMVectorSet(1.f, 0.f, 0.f, 0.f));
					dxAccum = 0;
				}
			}

			if (pickedAxis->pickedAxis == PickedAxis::Y)
			{
				dyAccum += dy;
				if (dyAccum > 0.5f)
				{
					actor->Move(-moveIncrement, XMVectorSet(0.f, 1.f, 0.f, 0.f));
					dyAccum = 0;
				}
				else if (dyAccum < -0.5f)
				{
					actor->Move(moveIncrement, XMVectorSet(0.f, 1.f, 0.f, 0.f));
					dyAccum = 0;
				}
			}

			if (pickedAxis->pickedAxis == PickedAxis::Z)
			{
				dzAccum += dx;
				if (dzAccum > 0.5f)
				{
					actor->Move(-moveIncrement, XMVectorSet(0.f, 0.f, 1.f, 0.f));
					dzAccum = 0;
				}
				else if (dzAccum < -0.5f)
				{
					actor->Move(moveIncrement, XMVectorSet(0.f, 0.f, 1.f, 0.f));
					dzAccum = 0;
				}
			}
		}
		else if (actor)
		{
			//TODO: There's a better way to do this
			//Couldcheck angle between camera and center point of transform widet for axis translations across different axis
			//For free movement
			if (axis == PickedAxis::X)
			{
				//these if()s are here to do correct translation based on camera pos
				if (editorCamera.location.m128_f32[2] > pickedActor->GetPositionFloat3().z) 
				{
					actor->Move(-dx * pickedActorMoveSpeed, XMVectorRight());
				}
				else
				{
					actor->Move(dx * pickedActorMoveSpeed, XMVectorRight());
				}
			}
			else if (axis == PickedAxis::Y)
			{
				actor->Move(-dy * pickedActorMoveSpeed, XMVectorUp());
			}	
			else if (axis == PickedAxis::Z)
			{
				if (editorCamera.location.m128_f32[0] > pickedActor->GetPositionFloat3().x)
				{
					actor->Move(dx * pickedActorMoveSpeed, XMVectorForward());
				}
				else
				{
					actor->Move(-dx * pickedActorMoveSpeed, XMVectorForward());
				}
			}
		}

		if (actor)
		{
			xAxis.actors[0].transform = actor->transform;
			yAxis.actors[0].transform = actor->transform;
			zAxis.actors[0].transform = actor->transform;
		}

		ReleaseCapture();
	}

	lastMousePosX = mouseX;
	lastMousePosY = mouseY;
}
