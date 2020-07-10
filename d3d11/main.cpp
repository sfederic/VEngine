//putting here for malloc_dbg
//#define _DEBUG 

#include "DXUtil.h"
#include "Win32Util.h"
#include "UIContext.h"
#include "Obj.h"
#include "Camera.h"
#include "Audio.h"
#include "AudioContext.h"
#include "Input.h"
#include "WICTextureLoader.h"
#include "Actor.h"
#include <thread>
#include <omp.h>

Win32Util win32;
DXUtil dx;
UIContext ui;
AudioContext ac;

XMVECTOR rayOrigin;
XMVECTOR rayDir;

void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance, ID3D11Buffer* debugBuffer)
{
	Vertex v1 = {}, v2 = {};
	XMStoreFloat3(&v1.pos, rayOrigin);
	XMVECTOR rayEnd = rayOrigin + (rayDir * distance);
	XMStoreFloat3(&v2.pos, rayEnd);

	dx.debugLines.push_back(v1);
	dx.debugLines.push_back(v2);

	dx.context->UpdateSubresource(debugBuffer, 0, nullptr, dx.debugLines.data(), 0, 0);
}

//TODO: where do I put this?
void Raycast(int sx, int sy, Camera* camera, XMMATRIX& worldMatrix)
{
	XMMATRIX V = camera->view;
	float vx = (+2.0f * sx / windowWidth - 1.0f) / V.r[0].m128_f32[0]; 
	float vy = (-2.0f * sy / windowHeight + 1.0f) / V.r[1].m128_f32[1];

	rayOrigin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	rayDir = XMVectorSet(vx, vy, 1.f, 0.f); //TODO: need camera forward vector here in z?
	
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V); 
	XMMATRIX W = worldMatrix; 
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);
	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);
	rayDir = XMVector3Normalize(rayDir);
}

void FrustumCullTest(Camera& camera, ActorSystem& system)
{
	//Is openmp even doing anything here?
	//is SIMD running in Debug build?
	#pragma omp parallel for
	for (int i = 0; i < system.actors.size(); i++)
	{
		XMMATRIX view = camera.view;
		XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);

		XMMATRIX world = system.actors[i].transform;
		XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);

		XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);

		BoundingFrustum frustum, localSpaceFrustum;
		BoundingFrustum::CreateFromMatrix(frustum, camera.proj);
		frustum.Transform(localSpaceFrustum, viewToLocal);

		system.boundingBox.Center = system.actors[i].GetPositionFloat3();
		system.boundingBox.Extents = system.actors[i].GetScale();

		if (localSpaceFrustum.Contains(system.boundingBox) == DISJOINT)
		{
			system.actors[i].bRender = false;
		}
		else
		{
			system.actors[i].bRender = true;
		}
	}
}

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	//WIN32 SETUP
	win32.SetupWindow(instance, cmdShow);
	win32.SetTimerFrequency();

	//D3D11 SETUP
	dx.CreateDevice();
	dx.CreateSwapchain();
	dx.CreateRTVAndDSV();
	dx.CreateShaders();

	//TODO: move to renderer. need to figure out shader generation per actor 
	//Test shader reflection.
	//No need to generate  or reflect buffers, just grab name as link to static struct name that holds the buffer inside of it and be done
	/*
	struct cbuffertest
	{
		name; //just make it an id
		cbuffer_data;
	}
	*/
	/*{	
		ID3D11ShaderReflection* cbReflection;
		HR(D3DReflect(dx.vertexCode->GetBufferPointer(), dx.vertexCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&cbReflection));
		D3D11_SHADER_BUFFER_DESC shaderDesc = {};
		ID3D11ShaderReflectionConstantBuffer* cb = cbReflection->GetConstantBufferByIndex(0);
		cb->GetDesc(&shaderDesc);

		for (int i = 0; i < shaderDesc.Variables; i++)
		{
			ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(i);
			D3D11_SHADER_VARIABLE_DESC varDesc;
			var->GetDesc(&varDesc);

			ID3D11ShaderReflectionType* type = var->GetType();
			D3D11_SHADER_TYPE_DESC typeDesc;
			type->GetDesc(&typeDesc);
		}

	}*/

	dx.CreateInputLayout();
	dx.CreateRasterizerStates();

	Camera camera(XMVectorSet(0.f, 0.f, -5.f, 1.f));

	dx.CreateConstantBuffer(camera);

	//Create test profiling
	D3D11_QUERY_DESC qd = {};
	qd.Query = D3D11_QUERY_TIMESTAMP;
	HR(dx.device->CreateQuery(&qd, &dx.startTimeQuery));
	HR(dx.device->CreateQuery(&qd, &dx.endTimeQuery));

	qd.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	HR(dx.device->CreateQuery(&qd, &dx.disjointQuery));

	D3D11_COUNTER_DESC counterDesc = {};
	counterDesc.Counter = D3D11_COUNTER_DEVICE_DEPENDENT_0;
	HR(dx.device->CreateCounter(&counterDesc, &dx.gpuCounter));

	//AUDIO SETUP
	ac.Init();

	//UI SETUP
	ui.init(dx.swapchain);

	//TEXTURE TESTING
	//still wondering if I should write WIC loader myself for texture struct
	ID3D11Resource* testTexture;
	ID3D11ShaderResourceView* testSrv;
	HR(CreateWICTextureFromFile(dx.device, L"texture.png", &testTexture, &testSrv));
	dx.context->PSSetShaderResources(0, 1, &testSrv);

	//TODO: move into dxutil
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	ID3D11SamplerState* testSampler;
	dx.device->CreateSamplerState(&sampDesc, &testSampler);
	dx.context->PSSetSamplers(0, 1, &testSampler);

	//ACTOR SYSTEM TESTING
	ActorSystem system;
	system.CreateActors("Models/cube.obj", &dx, 2);
	//See if threading is worthwhile. Is is slowing down the main thread somehow?
	//std::thread thread1(&ActorSystem::CreateActors, &system, "Models/sphere.obj", &dx, 4); //Did I get 96,000 draw calls in release build?
	//thread1.join();

	ID3D11Buffer* debugLinesBuffer = dx.CreateDefaultBuffer(sizeof(Vertex) * 32, D3D11_BIND_VERTEX_BUFFER, debugLineData);

	//MAIN LOOP
	while (msg.message != WM_QUIT) 
	{
		win32.StartTimer();

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//UI UPDATE
		ui.update();

		//TODO: put into engine tick or renderer tick or something
		if (GetKeyUpState('1'))
		{
			dx.context->RSSetState(dx.rastStateWireframe);
		}
		if (GetKeyUpState('2'))
		{
			dx.context->RSSetState(dx.rastStateSolid);
		}


		if (GetMouseDownState())
		{
			for (int i = 0; i < system.actors.size(); i++)
			{
				Raycast(ui.mousePos.x, ui.mousePos.y, &camera, system.actors[i].transform);
				float dist = 10000.f;
				//system.boundingBox.Center = system.actors[i].GetPositionFloat3();
				//system.boundingBox.Extents = system.actors[i].GetScale();
				if (system.actors[i].boundingBox.Intersects(rayOrigin, rayDir, dist))
				{
					DrawRayDebug(rayOrigin, rayDir, dist, debugLinesBuffer);
					OutputDebugString("hit");

					break;
				}
			}
		}

		if (GetAsyncKeyState(VK_RIGHT))
		{
			XMVECTOR pos = system.actors[0].GetPositionVector();
			pos.m128_f32[0] += 0.15f;
			system.actors[0].SetPosition(pos);
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			XMVECTOR pos = system.actors[0].GetPositionVector();
			pos.m128_f32[0] -= 0.15f;
			system.actors[0].SetPosition(pos);
		}


		//TODO: move All below into cammera Tick

		//FrustumCullTest(camera, system);
		camera.MouseMove(ui.mousePos.x, ui.mousePos.y);
		camera.UpdateViewMatrix();

		if (GetAsyncKeyState('W'))
		{
			camera.MoveForward(5.f * win32.delta);
		}
		if (GetAsyncKeyState('S'))
		{
			camera.MoveForward(-5.f * win32.delta);
		}
		if (GetAsyncKeyState('D'))
		{
			camera.Strafe(5.f * win32.delta);
		}
		if (GetAsyncKeyState('A'))
		{
			camera.Strafe(-5.f * win32.delta);
		}
		if (GetAsyncKeyState('Q'))
		{
			camera.MoveUp(-5.f * win32.delta);
		}
		if (GetAsyncKeyState('E'))
		{
			camera.MoveUp(5.f * win32.delta);
		}



		//RENDER
		dx.Render(&camera, &ui, &system, &dx, debugLinesBuffer);


		win32.EndTimer();
	}

	ui.cleanup();

	return (int)msg.wParam;
}
