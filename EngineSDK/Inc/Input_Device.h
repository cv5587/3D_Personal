#ifndef  InputDev_h__
#define InputDev_h__

#include "Base.h"

#define KEYDOWN(name, key) (name[key] & 0x80) 
#define MOUSEDOWN(name, key) (name[key] & 0x80) 



BEGIN(Engine)
class ENGINE_DLL CInput_Device :
    public CBase
{

private:
	CInput_Device(void);
	virtual ~CInput_Device(void) = default;

public:
	_byte	Get_DIKeyState(_ubyte byKeyID) 	{
		return KEYDOWN(m_byKeyState,byKeyID); 
	}
	_byte		Get_DIKeyState_Once(_ubyte byKeyID);

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)	{
		return MOUSEDOWN(m_tMouseState.rgbButtons, eMouse);
	}

	_byte		Get_DIMouseState_Once(MOUSEKEYSTATE eMouse);

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	DIMOUSESTATE			m_tMouseState;
	_bool m_bKeyDown = false;
	_bool m_bMouseDown = false;
public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void	Free(void);

};

END
#endif//InputDev_h_