#pragma once

#pragma warning (disable : 4251)

#include <d3d11.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>
#include "fx11\d3dx11effect.h"
#include "DirectXTK\DDSTextureLoader.h"
#include "DirectXTK\WICTextureLoader.h"

#include <string>
using namespace DirectX;

#include <vector>
#include <list>
#include <map>
#include <algorithm>

namespace Engine
{
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };
}

#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"



#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

using namespace Engine;
using namespace std;

