#pragma once

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)
#pragma warning (disable : 4244)

#include <d3d11.h>

#include <Effects11/d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/ScreenGrab.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/Effects.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <DirectXCollision.h>
#include <DirectXMath.h>

#include <d3dcompiler.h>

//#include <typeinfo.h>

using namespace DirectX;

#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>

#include <process.h>

#include <vector>
#include <list>
#include <map>
#include <tuple>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <functional>
#include <Queue>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <assert.h>
#include <io.h>
#include <math.h>

/* For.Gdiplus*/
#include <Gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

/* For.PhysX*/
#include "PhysX/PxPhysics.h"
#include "PhysX/PxPhysicsAPI.h"
using namespace physx;

/* For.Json*/
#include "json.hpp"
using	json = nlohmann::json;

/* For.ComPtr*/
#include <wrl.h>
using namespace Microsoft::WRL;
using namespace std;



#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"

#ifdef _DEBUG
#define USE_IMGUI
#define _COLLIDER_RENDER
#endif


#if !defined(USE_IMGUI)
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif // DBG_NEW 
#endif // _DEBUG
#endif // !defined(USE_IMGUI)


using namespace Engine;



