#include "State_FlarePre.h"
#include "Player.h"
CState_FlarePre::CState_FlarePre()
	:CState{}
{
}

void CState_FlarePre::Enter(CPlayer* Player)
{
}

void CState_FlarePre::Update(CPlayer* Player, _float fTimeDelta)
{
	if (Player->isAnimFinished())
	{

		Player->Set_State(PLAYERSTATE::PLAYER_FLARESUCCESS);
		

		Player->End_Change();
	}
	_long		MouseMoveX = { 0 };
	if (MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		Player->Player_Turn(fTimeDelta, MouseMoveX);
	}

	_long		MouseMoveY = { 0 };
	if (MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		Player->Cam_Turn(fTimeDelta, MouseMoveY);
	}

	Player->Mouse_Fix();
}

void CState_FlarePre::Exit(CPlayer* Player)
{
}

CState* CState_FlarePre::Create()
{
	CState_FlarePre* pInstance = new CState_FlarePre();
	return pInstance;
}

void CState_FlarePre::Free()
{
	__super::Free();
}
