#include "State_Walk_E.h"
#include "Player.h"

CState_Walk_E::CState_Walk_E()
    :CState{}
{
}

void CState_Walk_E::Enter(CPlayer* Player)
{
}

void CState_Walk_E::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIMouseState(DIM_RB))
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
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
			Player->Go_Left(fTimeDelta);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
			Player->Go_Right(fTimeDelta);
		}
	}
	else
	{
		Player->Set_State(PLAYERSTATE::PLAYER_AIM_E);
	}

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
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_S);
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
					Player->Set_State(PLAYERSTATE::PLAYER_WALK_S);

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
				Player->Set_State(PLAYERSTATE::PLAYER_AIM);
			}
		}
		else
			Player->Set_State(PLAYERSTATE::PLAYER_AIM_E);

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

void CState_Walk_E::Exit(CPlayer* Player)
{
}

CState* CState_Walk_E::Create()
{
	CState_Walk_E* pInstance = new CState_Walk_E();
    return pInstance;
}

void CState_Walk_E::Free()
{
	__super::Free();
}
