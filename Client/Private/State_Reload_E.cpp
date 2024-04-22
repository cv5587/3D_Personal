#include "State_Reload_E.h"
#include "Player.h"

CState_Reload_E::CState_Reload_E()
	:CState{}
{
}

void CState_Reload_E::Enter(CPlayer* Player)
{
}

void CState_Reload_E::Update(CPlayer* Player, _float fTimeDelta)
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

	if (Player->isAnimFinished() && Player->isRevolver_AnimFin())
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
				Player->Set_State(PLAYERSTATE::PLAYER_AIM_S);
			}
		}
		else
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_W))
			{
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
				Player->Set_State(PLAYERSTATE::PLAYER_AIM_S);
			}
		}


		Player->End_Change();
	}
}

void CState_Reload_E::Exit(CPlayer* Player)
{
}

CState* CState_Reload_E::Create()
{
	CState_Reload_E* pInstance = new CState_Reload_E();
	return pInstance;
}

void CState_Reload_E::Free()
{
	__super::Free();
}
