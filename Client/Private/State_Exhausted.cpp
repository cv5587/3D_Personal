#include "State_Exhausted.h"
#include "Player.h"
//탈진상태 제작후 탈진 받아와서 조건 잡아주기.
CState_Exhausted::CState_Exhausted()
	:CState{}
{
}

void CState_Exhausted::Enter(CPlayer* Player)
{
	
}

void CState_Exhausted::Update(CPlayer* Player, _float fTimeDelta)
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

	if (m_pGameInstance->Get_DIKeyState(DIK_W) && m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
	{
		Player->Set_State(PLAYERSTATE::PLAYER_RUN);
	}
}

void CState_Exhausted::Exit(CPlayer* Player)
{
}

CState* CState_Exhausted::Create()
{
	CState_Exhausted* pInstance = new CState_Exhausted();
	return pInstance;
}

void CState_Exhausted::Free()
{
	__super::Free();
}
