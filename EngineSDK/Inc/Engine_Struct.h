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
	
}
#endif // Engine_Struct_h__
