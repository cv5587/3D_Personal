#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	/* 엔진을 초기화하기위해 필요한 데이터 여러개를 묶었다. */
	typedef struct 
	{
		bool			isWindowed;
		HWND			hWnd;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
	}ENGINE_DESC;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;

	typedef struct 
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE		eType = { TYPE_END };

		XMFLOAT4	vDirection;
		XMFLOAT4	vPosition;
		float		fRange;

		/* 빛의 기본적인 색상. */
		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;
		_bool			bSwitch;
	}LIGHT_DESC;

	typedef struct 
	{
		class CTexture* MaterialTextures[AI_TEXTURE_TYPE_MAX];
		_bool isTextured;
	}MESH_MATERIAL;

	typedef struct 
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		double			Time;
	}KEYFRAME;

	typedef struct ENGINE_DLL 
	{
		XMFLOAT3		vPosition;

		static const unsigned int		iNumElements = { 1 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[1];

	}VTXPOS;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;

		static const unsigned int		iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[2];
	}VTXPOINT;

	typedef struct ENGINE_DLL  
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int		iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[2];

	}VTXPOSTEX;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;

		static const unsigned int		iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[2];

	}VTXCUBE;

	typedef struct ENGINE_DLL 
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int		iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[3];

	}VTXNORTEX;

	typedef struct ENGINE_DLL 
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;  //새로운거임

		static const unsigned int		iNumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[4];

	}VTXMESH;

	typedef struct ENGINE_DLL  VTXANIMMESH
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		XMUINT4			vBlendIndices;
		XMFLOAT4		vBlendWeights;

		static const unsigned int		iNumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[6];

	}VTXANIMMESH;

	typedef struct ENGINE_DLL
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
		XMFLOAT2			vLifeTime;
	}VTXMATRIX;

	typedef struct ENGINE_DLL
	{
		static const unsigned int		iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[7];
	}VTXINSTANCE_RECT;

	typedef struct ENGINE_DLL
	{
		static const unsigned int		iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[7];
	}VTXINSTANCE_POINT;


}
#endif // Engine_Struct_h__
