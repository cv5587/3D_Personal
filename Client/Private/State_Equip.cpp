#include "State_Equip.h"
#include "Player.h"

CState_Equip::CState_Equip()
	:CState{}
{
}

void CState_Equip::Enter(CPlayer* Player)
{


}

void CState_Equip::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_W))
	{
		Player->Go_Straight(fTimeDelta);
	}
	else 	if (m_pGameInstance->Get_DIKeyState(DIK_S))
	{
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

	if(Player->isAnimFinished())
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_W))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_S))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_JOG);
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
		Player->End_Change();
	}
}

void CState_Equip::Exit(CPlayer* Player)
{
	
}

CState* CState_Equip::Create()
{
	CState_Equip* pInstance = new CState_Equip();
	return pInstance;
}

void CState_Equip::Free()
{
	__super::Free();
}
