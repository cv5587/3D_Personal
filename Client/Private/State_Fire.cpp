#include "State_Fire.h"
#include "Player.h"

CState_Fire::CState_Fire()
    :CState{}
{
}

void CState_Fire::Enter(CPlayer* Player)
{
	//탄창 애니메이션 진행
}

void CState_Fire::Update(CPlayer* Player, _float fTimeDelta)
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
			Player->Set_State(PLAYERSTATE::PLAYER_AIM);
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

void CState_Fire::Exit(CPlayer* Player)
{
	//리볼버 탄창 관리
}

CState* CState_Fire::Create()
{
    CState_Fire* pInstance = new CState_Fire();
    return pInstance;
}

void CState_Fire::Free()
{
    __super::Free();
}
