#include "State_Walk_LE.h"
#include "Player.h"

CState_Walk_LE::CState_Walk_LE()
    :CState{}
{
}

void CState_Walk_LE::Enter(CPlayer* Player)
{
}

void CState_Walk_LE::Update(CPlayer* Player, _float fTimeDelta)
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
				Player->Set_State(PLAYERSTATE::PLAYER_AIM_S);
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

void CState_Walk_LE::Exit(CPlayer* Player)
{
}

CState* CState_Walk_LE::Create()
{
	CState_Walk_LE* pInstance = new CState_Walk_LE();
    return pInstance;
}

void CState_Walk_LE::Free()
{
	__super::Free();
}
