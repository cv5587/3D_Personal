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
	if(Player->isEquip() != PLAYEREQUIP::EQUIP_RABBIT)
	{

		if (m_pGameInstance->Get_DIKeyState(DIK_W))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT))
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


		if (m_pGameInstance->Get_DIKeyState_Once(DIK_I))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_INVENTORY);
			Player->Mouse_Fix();
			return;
		}

		if (Player->isEquip() != PLAYEREQUIP::EQUIP_RABBIT)
		{
			if (m_pGameInstance->Get_DIKeyState_Once(DIK_B))
			{
				Player->Set_Equip(PLAYEREQUIP::EQUIP_RABBIT);
				Player->Set_State(PLAYERSTATE::PLAYER_EQUIP);
			}
			else if (m_pGameInstance->Get_DIKeyState_Once(DIK_J))
			{
				Player->Set_Equip(PLAYEREQUIP::EQUIP_REVOLVER);
				Player->Set_State(PLAYERSTATE::PLAYER_EQUIP);
			}
		}

		if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
		{

			if (Player->Pick_up())
			{
				if(!Player->isRabbitCatch())
				{
					Player->Set_State(PLAYERSTATE::PLAYER_PICKUP);
					Player->Mouse_Fix();
				}
				else
				{
					Player->Set_Equip(PLAYEREQUIP::EQUIP_RABBIT);
					Player->Set_State(PLAYERSTATE::PLAYER_EQUIP);
					Player->Mouse_Fix();
				}
				return;
			}			
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










		if (m_pGameInstance->Get_DIKeyState_Once(DIK_R))
			if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
				Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_S);

		if (m_pGameInstance->Get_DIMouseState(DIM_RB))
		{
			if (Player->isEquip() != PLAYEREQUIP::EQUIP_NONE )
				Player->Set_State(PLAYERSTATE::PLAYER_AIM_S);
		}

		if (m_pGameInstance->Get_DIMouseState(DIM_LB))
		{
			if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
				Player->Set_State(PLAYERSTATE::PLAYER_THROW);
			else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
				Player->Set_State(PLAYERSTATE::PLAYER_HIPFIRE);
		}
	}
	else
	{
		if (m_pGameInstance->Get_DIMouseState(DIM_LB))
		{
				Player->Set_State(PLAYERSTATE::PLAYER_KILL);
		}
		if (m_pGameInstance->Get_DIMouseState(DIM_RB))
		{
				Player->Set_State(PLAYERSTATE::PLAYER_RELEASE);
		}
	}

	Player->Mouse_Fix();
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
