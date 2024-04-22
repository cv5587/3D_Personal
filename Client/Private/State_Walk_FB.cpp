#include "State_Walk_FB.h"
#include "Player.h"

CState_Walk_FB::CState_Walk_FB()
	:CState{}
{
}

void CState_Walk_FB::Enter(CPlayer* Player)
{
}

void CState_Walk_FB::Update(CPlayer* Player, _float fTimeDelta)
{

	if (m_pGameInstance->Get_DIMouseState(DIM_RB))
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_W))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_A))
			{
				if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_S);
				else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_L);
			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_D))
			{
				if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_S);
				else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_R);
			}
			else
				Player->Go_Straight(fTimeDelta);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_S))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_A))
			{
				if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_S);
				else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_L);
			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_D))
			{
				if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_S);
				else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_R);
			}
			else
				Player->Go_Backward(fTimeDelta);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
			if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_S);
			else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_L);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
			if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_S);
			else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_R);
		}
		else
		{
			if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_E);
			else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
				Player->Set_State(PLAYERSTATE::PLAYER_AIM);
		}

		if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
		{
			if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
				Player->Set_State(PLAYERSTATE::PLAYER_THROW);
			else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
				Player->Set_State(PLAYERSTATE::PLAYER_FIRE);
		}

	}
	else
	{
		Player->Set_State(PLAYERSTATE::PLAYER_AIM_E);
	}
}

void CState_Walk_FB::Exit(CPlayer* Player)
{
}

CState* CState_Walk_FB::Create()
{
	CState_Walk_FB* pInstance = new CState_Walk_FB();
	return pInstance;
}

void CState_Walk_FB::Free()
{
	__super::Free();
}
