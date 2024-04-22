#include "State_Aim_S.h"
#include "Player.h"
CState_Aim_S::CState_Aim_S()
    :CState{}
{
}

void CState_Aim_S::Enter(CPlayer* Player)
{
}

void CState_Aim_S::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_W))
	{
		Player->Go_Straight(fTimeDelta);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_S))
	{
		Player->Go_Backward(fTimeDelta);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_A))
	{
		Player->Go_Left(fTimeDelta);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_D))
	{
		Player->Go_Right(fTimeDelta);
	}

	if (m_pGameInstance->Get_DIMouseState(DIM_RB) )
	{
		Player->Set_State(PLAYERSTATE::PLAYER_AIM_S);
	}
	else
	{
		if (CON_NORMAL == Player->isCondition())
		{
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
		}
		else
		{
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE_EXHAUSTED);
		}
	}

	if (Player->isAnimFinished())
	{
		if (m_pGameInstance->Get_DIMouseState(DIM_RB))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_AIM);
		}

		Player->End_Change();
	}
}

void CState_Aim_S::Exit(CPlayer* Player)
{
}

CState* CState_Aim_S::Create()
{
	CState_Aim_S* pInstance = new CState_Aim_S();
	return pInstance;
}

void CState_Aim_S::Free()
{
	__super::Free();
}
