#include "WorldEditor.h"
#include "Input.h"
#include "Camera.h"
#include "UISystem.h"
#include "World.h"
#include "CoreSystem.h"
#include "ShaderFactory.h"

WorldEditor gWorldEditor;

void WorldEditor::Tick()
{
	//Actor picking for editor
	if (inputSystem.GetMouseLeftDownState())
	{
		if (RaycastAllFromScreen(screenPickRay, gUISystem.mousePos.x, gUISystem.mousePos.y, &editorCamera, GetWorld()))
		{
			xAxis.actors[0].bRender = true;
			yAxis.actors[0].bRender = true;
			zAxis.actors[0].bRender = true;

			actorIndex = screenPickRay.actorIndex;
			actorSystemIndex = screenPickRay.actorSystemIndex;

			World* world = GetWorld();
			xAxis.actors[0].SetPosition(world->GetActor(actorSystemIndex, actorIndex)->GetPositionVector());
			yAxis.actors[0].SetPosition(world->GetActor(actorSystemIndex, actorIndex)->GetPositionVector());
			zAxis.actors[0].SetPosition(world->GetActor(actorSystemIndex, actorIndex)->GetPositionVector());
		}
	}
	else if (inputSystem.GetMouseRightUpState())
	{
		if (RaycastAllFromScreen(screenPickRay, gUISystem.mousePos.x, gUISystem.mousePos.y, &editorCamera, GetWorld()))
		{
			actorIndex = screenPickRay.actorIndex;
			actorSystemIndex = screenPickRay.actorSystemIndex;

			//Floating window for actor stats
			gUISystem.AddView(L"ActorTest", actorSystemIndex, actorIndex);
		}
	}

	//TODO: this is no good
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

	axes.push_back(&xAxis);
	axes.push_back(&yAxis);
	axes.push_back(&zAxis);
}

void WorldEditor::MoveActor(Actor* actor)
{
	static POINT lastMousePos;

	int x = gUISystem.mousePos.x;
	int y = gUISystem.mousePos.y;

	if (inputSystem.GetAsyncKey(VK_LBUTTON))
	{
		SetCapture(gCoreSystem.mainWindow);

		float dx = XMConvertToRadians(0.25f * (float)(x - lastMousePos.x));
		float dy = XMConvertToRadians(0.25f * (float)(y - lastMousePos.y));

		if (bMoveActorsInIncrements)
		{
			//For incremental movement
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
		else
		{
			//For free movement
			actor->Move(dx * pickedActorMoveSpeed, XMVectorSet(1.f, 0.f, 0.f, 0.f));
			actor->Move(-dy * pickedActorMoveSpeed, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		}

		ReleaseCapture();
	}

	lastMousePos.x = x;
	lastMousePos.y = y;
}
