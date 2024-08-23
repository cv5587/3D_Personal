#include "State_Build.h"
#include "Player.h"
CState_Build::CState_Build()
	:CState{}
{
}

void CState_Build::Enter(CPlayer* Player)
{
}

void CState_Build::Update(CPlayer* Player, _float fTimeDelta)
{
	Player->Move_BuildObject(fTimeDelta);
	if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
	{
		Player->BuildObject();
		Player->Set_State(PLAYER_IDLE);
		return;
	}
	_long		MouseMoveY = { 0 };
	if (MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		Player->Cam_Turn(fTimeDelta, MouseMoveY);
	}

	_long		MouseMoveX = { 0 };
	if (MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		Player->Player_Turn(fTimeDelta, MouseMoveX);
	}
	Player->Mouse_Fix();
}

void CState_Build::Exit(CPlayer* Player)
{

}

CState* CState_Build::Create()
{
	CState_Build* pInstance = new CState_Build();
	return pInstance;
}

void CState_Build::Free()
{
	__super::Free();
}
