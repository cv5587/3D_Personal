#include "State_Throw.h"
#include "Player.h"

CState_Throw::CState_Throw()
	:CState{}
{
}

void CState_Throw::Enter(CPlayer* Player)
{
}

void CState_Throw::Update(CPlayer* Player, _float fTimeDelta)
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



	if (Player->isAnimFinished())
	{
		Player->Set_UnEquip();
		Player->End_Change();
		if (Player->isCondition() == PLAYERCONDITION::CON_NORMAL)
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
		else
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE_EXHAUSTED);
	}
}

void CState_Throw::Exit(CPlayer* Player)
{
}

CState* CState_Throw::Create()
{
	CState_Throw* pInstance = new CState_Throw();
	return pInstance;
}

void CState_Throw::Free()
{
	__super::Free();
}
