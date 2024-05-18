#include "State_Jog.h"
#include "Player.h"
CState_Jog::CState_Jog()
	:CState{}
{
}

void CState_Jog::Enter(CPlayer* Player)
{
	if(Player->isCondition()==CON_NORMAL)
		Player->Set_DeltaValue(SPEED_JOG, XMConvertToRadians(120.0f));
	else
		Player->Set_DeltaValue(SPEED_EX_JOG, XMConvertToRadians(120.0f));
}

void CState_Jog::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_W))
	{
		Player->Set_State(PLAYERSTATE::PLAYER_JOG);
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
		Player->Set_State(PLAYERSTATE::PLAYER_JOG);
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
		Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		Player->Go_Left(fTimeDelta);
	}
	else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
	{
		Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		Player->Go_Right(fTimeDelta);
	}
	else
	{
		Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
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

	if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
	{
		if (Player->Pick_up())
			Player->Set_State(PLAYERSTATE::PLAYER_PICKUP);
	}



	 if (m_pGameInstance->Get_DIMouseState(DIM_RB))
		{
			if (Player->isEquip() != PLAYEREQUIP::EQUIP_NONE)
				Player->Set_State(PLAYERSTATE::PLAYER_AIM_S);
		}
	 if (m_pGameInstance->Get_DIKeyState(DIK_R))
		{
			if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
				Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_S);
		}

	 if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT) && m_pGameInstance->Get_DIKeyState(DIK_W))
	{
			Player->Set_State(PLAYERSTATE::PLAYER_RUN);
	}

	 if (m_pGameInstance->Get_DIMouseState(DIM_LB))
	 {
		 if (Player->isEquip() == PLAYEREQUIP::EQUIP_STONE)
			 Player->Set_State(PLAYERSTATE::PLAYER_THROW);
		 else if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
			 Player->Set_State(PLAYERSTATE::PLAYER_HIPFIRE);
	 }

	 if (Player->isEquipChange())
	 {
		 if(Player->isEquip() == PLAYEREQUIP::EQUIP_NONE)
			 Player->Set_State(PLAYERSTATE::PLAYER_EQUIP);
		 else
			 Player->Set_State(PLAYERSTATE::PLAYER_UNEQUIP);
	 }
	 Player->Mouse_Fix();
}

void CState_Jog::Exit(CPlayer* Player)
{

}

CState* CState_Jog::Create()
{
	CState_Jog* pInstance = new CState_Jog();
	return pInstance;
}

void CState_Jog::Free()
{
	__super::Free();
}
