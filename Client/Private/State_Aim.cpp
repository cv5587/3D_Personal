#include "State_Aim.h"
#include "Player.h"

CState_Aim::CState_Aim()
    :CState{}
{
}

void CState_Aim::Enter(CPlayer* Player)
{

}

void CState_Aim::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIMouseState(DIM_RB))
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_W))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_A))
			{
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_D))
			{
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
			}
			else
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_S))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_A))
			{
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
			}
			else if (m_pGameInstance->Get_DIKeyState(DIK_D))
			{
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
			}
			else
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);

		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_L);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_R);
		}

		if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
		{
			if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
				Player->Set_State(PLAYERSTATE::PLAYER_THROW);
			else if(Player->isEquip()==PLAYEREQUIP::EQUIP_REVOLVER)
				Player->Set_State(PLAYERSTATE::PLAYER_FIRE);
		}

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

void CState_Aim::Exit(CPlayer* Player)
{
}

CState* CState_Aim::Create()
{
	CState_Aim* pInstance = new CState_Aim();
	return pInstance;
}

void CState_Aim::Free()
{
	__super::Free();
}
