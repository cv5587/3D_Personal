#include "State_Hipfire.h"
#include "Player.h"

CState_Hipfire::CState_Hipfire()
	:CState{}
{
}

void CState_Hipfire::Enter(CPlayer* Player)
{
}

void CState_Hipfire::Update(CPlayer* Player, _float fTimeDelta)
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
		Player->End_Change();
		Player->isFire();
		if (m_pGameInstance->Get_DIMouseState(DIM_RB))
			Player->Set_State(PLAYERSTATE::PLAYER_AIM_S);
		else
			if (Player->isCondition() == PLAYERCONDITION::CON_NORMAL)
				Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
			else
				Player->Set_State(PLAYERSTATE::PLAYER_IDLE_EXHAUSTED);
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

void CState_Hipfire::Exit(CPlayer* Player)
{
}

CState* CState_Hipfire::Create()
{
	CState_Hipfire* pInstance = new CState_Hipfire();
	return pInstance;
}

void CState_Hipfire::Free()
{
	__super::Free();
}
