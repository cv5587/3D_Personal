#pragma once

#pragma warning (disable : 4251)

#include <d3d11.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>

#include <random>

#include "fx11\d3dx11effect.h"
#include "DirectXTK\DDSTextureLoader.h"
#include "DirectXTK\WICTextureLoader.h"
#include "DirectXTK\ScreenGrab.h"//마스크
#include "DirectXTK\PrimitiveBatch.h"//콜라이더
#include "DirectXTK\VertexTypes.h"//콜라이더
#include "DirectXTK\SpriteBatch.h"//폰트
#include "DirectXTK\SpriteFont.h"//폰트
#include "DirectXTK\Effects.h"


#include "assimp\scene.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"

#include "fmod\fmod.h"
#include "fmod\fmod.hpp"
#include <io.h>
#include <iostream>
#include <string>
using namespace DirectX;

#include <array>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <fstream>

namespace Engine
{
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };
	enum class TextureType
	{
		HitIDTexture=0,

	};
	enum CHANNELID {
		BACKGROUND, FLARE, ENTER, MATCH, OPEN,INVENTORY, CH_WOLF,CH_GUN,CH_COOK, CH_STOVE,
		CH_FOOT, CH_RABBIT,CH_FX,
		MAXCHANNEL
	};
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

