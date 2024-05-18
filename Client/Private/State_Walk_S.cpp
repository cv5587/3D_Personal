#include "State_Walk_S.h"
#include "Player.h"
CState_Walk_S::CState_Walk_S()
    :CState{}
{
}

void CState_Walk_S::Enter(CPlayer* Player)
{
	if (CON_NORMAL == Player->isCondition())
	{
		Player->Set_DeltaValue(SPEED_WALK, XMConvertToRadians(90.f));
	}
	else
	{
		Player->Set_DeltaValue(SPEED_EX_WALK, XMConvertToRadians(90.f));
	}
}

void CState_Walk_S::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIMouseState(DIM_RB))
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_W))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_A))
				Player->Go_LeftStraight(fTimeDelta);
			else if (m_pGameInstance->Get_DIKeyState(DIK_D))
				Player->Go_RightStraight(fTimeDelta);
			else
				Player->Go_Straight(fTimeDelta);	
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_S))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_A))
				Player->Go_LeftBackward(fTimeDelta);
			else if (m_pGameInstance->Get_DIKeyState(DIK_D))
				Player->Go_RightBackward(fTimeDelta);
			else
				Player->Go_Backward(fTimeDelta);
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
			Player->Go_Left(fTimeDelta);
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
			Player->Go_Right(fTimeDelta);
		}
	}
	else
		Player->Set_State(PLAYERSTATE::PLAYER_AIM_E);

	if (Player->isAnimFinished())
	{
		if (m_pGameInstance->Get_DIMouseState(DIM_RB))
		{
			
			if (m_pGameInstance->Get_DIKeyState(DIK_W))
			{
				if (m_pGameInstance->Get_DIKeyState(DIK_A))
				{
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_L);
				}
				else if (m_pGameInstance->Get_DIKeyState(DIK_D))
				{
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_R);
				}
				else
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
			}
			else 	if (m_pGameInstance->Get_DIKeyState(DIK_S))
			{
				if (m_pGameInstance->Get_DIKeyState(DIK_A))
				{
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_L);
				}
				else if (m_pGameInstance->Get_DIKeyState(DIK_D))
				{
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_R);
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
			else
			{
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_E);
			}
		}
		Player->End_Change();
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

void CState_Walk_S::Exit(CPlayer* Player)
{
}

CState* CState_Walk_S::Create()
{
	CState_Walk_S* pInstance = new CState_Walk_S();
	return pInstance;
}

void CState_Walk_S::Free()
{
	__super::Free();
}
