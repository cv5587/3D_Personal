#include "State_Walk_L.h"
#include "Player.h"

CState_Walk_L::CState_Walk_L()
	:CState{}
{
}

void CState_Walk_L::Enter(CPlayer* Player)
{
}

void CState_Walk_L::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIMouseState(DIM_RB))
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_W))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_A))
			{
				if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_LE);
				else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_D))
			{
				if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_LE);
				else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
			}
			else
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_S))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_A))
			{
				if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_LE);
				else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_D))
			{
				if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_LE);
				else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
			}
			else
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
				Player->Go_Left(fTimeDelta);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
			if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_LE);
			else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_R);
		}
		else
		{
			Player->Set_State(PLAYERSTATE::PLAYER_WALK_LE);
		}

		if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
			Player->Set_State(PLAYERSTATE::PLAYER_FIRE);

	}
	else
	{
		Player->Set_State(PLAYERSTATE::PLAYER_AIM_E);
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

void CState_Walk_L::Exit(CPlayer* Player)
{
}

CState* CState_Walk_L::Create()
{
	CState_Walk_L* pInstance = new CState_Walk_L();
	return pInstance;
}

void CState_Walk_L::Free()
{
	__super::Free();
}
