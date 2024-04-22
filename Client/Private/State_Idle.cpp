#include "State_Idle.h"
#include "Player.h"
CState_Idle::CState_Idle()
	:CState{}
{
}

void CState_Idle::Enter(CPlayer* Player)
{

}

void CState_Idle::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_W))
	{
		if(m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
			Player->Set_State(PLAYERSTATE::PLAYER_RUN);
		else
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

	if (m_pGameInstance->Get_DIMouseState(DIM_RB))
	{
		if(Player->isEquip()!=PLAYEREQUIP::EQUIP_NONE)
			Player->Set_State(PLAYERSTATE::PLAYER_AIM_S);
	}


	if (m_pGameInstance->Get_DIKeyState_Once(DIK_R))
		if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_S);
	if (m_pGameInstance->Get_DIMouseState(DIM_LB))
	{	
		if(Player->isEquip()==PLAYEREQUIP::EQUIP_STONE)
			Player->Set_State(PLAYERSTATE::PLAYER_THROW);
		else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
			Player->Set_State(PLAYERSTATE::PLAYER_HIPFIRE);
	}
}

void CState_Idle::Exit(CPlayer* Player)
{
}

CState* CState_Idle::Create()
{
	CState_Idle* pInstance = new CState_Idle();
	return pInstance;
}

void CState_Idle::Free()
{
	__super::Free();
}
