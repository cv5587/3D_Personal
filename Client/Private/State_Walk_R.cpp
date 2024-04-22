#include "State_Walk_R.h"
#include "Player.h"
CState_Walk_R::CState_Walk_R()
	:CState{}
{
}

void CState_Walk_R::Enter(CPlayer* Player)
{
}

void CState_Walk_R::Update(CPlayer* Player, _float fTimeDelta)
{
	if (m_pGameInstance->Get_DIMouseState(DIM_RB))
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_W))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_A))
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_RE);
			else if (m_pGameInstance->Get_DIKeyState(DIK_D))
				Player->Go_RightStraight(fTimeDelta);
			else
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_S))
		{
			if (m_pGameInstance->Get_DIKeyState(DIK_A))
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_RE);
			else if (m_pGameInstance->Get_DIKeyState(DIK_D))
				Player->Go_RightStraight(fTimeDelta);
			else
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_FB);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_A))
		{
				Player->Set_State(PLAYERSTATE::PLAYER_WALK_RE);
		}
		else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
		{
				Player->Go_Right(fTimeDelta);
		}
		else
		{
			Player->Set_State(PLAYERSTATE::PLAYER_WALK_RE);
		}

		if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
			Player->Set_State(PLAYERSTATE::PLAYER_FIRE);

	}
	else
	{
		Player->Set_State(PLAYERSTATE::PLAYER_AIM_E);
	}
}

void CState_Walk_R::Exit(CPlayer* Player)
{
}

CState* CState_Walk_R::Create()
{
	CState_Walk_R* pInstance = new CState_Walk_R();
	return pInstance;
}

void CState_Walk_R::Free()
{
	__super::Free();
}
