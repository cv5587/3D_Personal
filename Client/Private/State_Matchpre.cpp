#include "State_Matchpre.h"
#include "Player.h"
CState_Matchpre::CState_Matchpre()
	:CState{}
{
}

void CState_Matchpre::Enter(CPlayer* Player)
{
}

void CState_Matchpre::Update(CPlayer* Player, _float fTimeDelta)
{
	if (Player->isAnimFinished())
	{
	
		if (CON_NORMAL == Player->isCondition())
		{
			Player->Set_State(PLAYERSTATE::PLAYER_MATSUCCESS);
		}
		else
		{
			Player->Set_State(PLAYERSTATE::PLAYER_MATSUCCESS);
		}
		
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

void CState_Matchpre::Exit(CPlayer* Player)
{
}

CState* CState_Matchpre::Create()
{
	CState_Matchpre* pInstance = new CState_Matchpre();
	return pInstance;
}

void CState_Matchpre::Free()
{
	__super::Free();
}
