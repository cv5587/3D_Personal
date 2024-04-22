#include "State_Aim_E.h"
#include "Player.h"

CState_Aim_E::CState_Aim_E()
	:CState{}
{
}

void CState_Aim_E::Enter(CPlayer* Player)
{
}

void CState_Aim_E::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_W))
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
			Player->Go_LeftStraight(fTimeDelta);
		}
		else if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
			Player->Go_RightStraight(fTimeDelta);
		}
		else
			Player->Go_Straight(fTimeDelta);
	}
	else 	if (m_pGameInstance->Get_DIKeyState(DIK_S))
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
			Player->Go_LeftBackward(fTimeDelta);
		}
		else if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
			Player->Go_RightBackward(fTimeDelta);
		}
		else
			Player->Go_Backward(fTimeDelta);

	}
	else 	if (m_pGameInstance->Get_DIKeyState(DIK_A))
	{
		Player->Go_Left(fTimeDelta);
	}
	else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
	{
		Player->Go_Right(fTimeDelta);
	}

	if (m_pGameInstance->Get_DIMouseState(DIM_RB))
	{
		Player->Set_State(PLAYERSTATE::PLAYER_AIM_S);
	}

	if (Player->isAnimFinished())
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_W))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);	
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_S))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		}
		else
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
		
		Player->End_Change();
	}

}

void CState_Aim_E::Exit(CPlayer* Player)
{
}

CState* CState_Aim_E::Create()
{

	CState_Aim_E* pInstance = new CState_Aim_E();
	return pInstance;
}

void CState_Aim_E::Free()
{
	__super::Free();
}
