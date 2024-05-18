#include "State_Reload_Loop.h"
#include "Player.h"

CState_Reload_Loop::CState_Reload_Loop()
	:CState{}
{
}

void CState_Reload_Loop::Enter(CPlayer* Player)
{
}

void CState_Reload_Loop::Update(CPlayer* Player, _float fTimeDelta)
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
	else if (m_pGameInstance->Get_DIKeyState(DIK_R))
		Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_E);

	//장전 매커니즘 제작 해서 넣어주기 
	//애니메이션 갯수 만큼 클래스 제작 or 갯수 받아와서 반복 재생 제작
	if (Player->isAnimFinished()&&Player->isRevolver_AnimFin())//
	{
		//Player->End_Change();
		//애니메이션 루프 한번이 끝날때마다 장전되는 총알갯수 갱신 
		Player->isReload();
		if (6!=Player->Get_BullletLeft())
		{
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD);
			return;
		}

		if (m_pGameInstance->Get_DIMouseState(DIM_LB))
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_E);
		else
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_E);
				// 장전 횟수를 모두 채우면 장전 끝내기.

		return;
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

void CState_Reload_Loop::Exit(CPlayer* Player)
{
}

CState* CState_Reload_Loop::Create()
{
	CState_Reload_Loop* pInstance = new CState_Reload_Loop();
	return pInstance;
}

void CState_Reload_Loop::Free()
{
	__super::Free();
}
