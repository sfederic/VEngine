#pragma once

///Need this #define for static linking REF:http://help.autodesk.com/cloudhelp/2018/ENU/FBX-Developer-Help/getting_started/installing_and_configuring/configuring_the_fbx_sdk_for_wind.html
#define FBXSDK_SHARED

#include <fbxsdk.h>
#include "Render/RenderTypes.h"

using namespace fbxsdk;

//TODO: need to come back through here and change the actor system references to work with meshcomponents

//REF: https://github.com/peted70/hololens-fbx-viewer/tree/master/HolographicAppForOpenGLES1/include
//REF: https://peted.azurewebsites.net/hololens-fbx-loading-c/

// The oficial docs on the FBX SDK (the current version is hard to find, google brings you to 2014 docs)
//REF: https://help.autodesk.com/view/FBX/2020/ENU/

// Great GameDev.net reference on using FBX SDK and animation.
//REF: https://www.gamedev.net/tutorials/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582/

//NOTE: For the most part, not going to use FBX models with lights, cameras and extra nodes. One model should suffice. Import() code reflects that.
//NOTE: Models needto be triangluated, not working with control points (see asserts() below)

struct FBXImporter
{
	FbxManager* manager;
	FbxIOSettings* ioSetting;
	FbxImporter* importer;
	FbxAnimEvaluator* animEvaluator;

	void Init();
	bool Import(std::string filename, MeshDataProxy* meshData);
	void ProcessAllChildNodes(FbxNode* node, MeshData* meshData);
};

extern FBXImporter fbxImporter;
