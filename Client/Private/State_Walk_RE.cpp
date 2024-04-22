#include "State_Walk_RE.h"
#include "Player.h" 

CState_Walk_RE::CState_Walk_RE()
    :CState{}
{
}

void CState_Walk_RE::Enter(CPlayer* Player)
{
}

void CState_Walk_RE::Update(CPlayer* Player, _float fTimeDelta)
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
				Player->Set_State(PLAYERSTATE::PLAYER_AIM);
			}
		}
		else
			Player->Set_State(PLAYERSTATE::PLAYER_AIM_E);

		Player->End_Change();
	}
}

void CState_Walk_RE::Exit(CPlayer* Player)
{
}

CState* CState_Walk_RE::Create()
{
	CState_Walk_RE* pInstance = new CState_Walk_RE();
	return pInstance;
}

void CState_Walk_RE::Free()
{
	__super::Free();
}
