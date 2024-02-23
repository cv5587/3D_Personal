#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	/* ������ �ʱ�ȭ�ϱ����� �ʿ��� ������ �������� ������. */
	typedef struct
	{
		bool			isWindowed;
		HWND			hWnd;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
	}ENGINE_DESC;
	
}
#endif // Engine_Struct_h__
