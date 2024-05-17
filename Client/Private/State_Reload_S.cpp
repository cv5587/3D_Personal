#include "State_Reload_S.h"
#include "Player.h"

CState_Reload_S::CState_Reload_S()
	:CState{}
{
}

void CState_Reload_S::Enter(CPlayer* Player)
{
}

void CState_Reload_S::Update(CPlayer* Player, _float fTimeDelta)
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

	_bool body = Player->isAnimFinished();
	_bool revolver= Player->isRevolver_AnimFin();
	if (Player->isAnimFinished() && Player->isRevolver_AnimFin())
	{

		//Player->End_Change();

		if (m_pGameInstance->Get_DIMouseState(DIM_LB))	
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_E);
		else if(m_pGameInstance->Get_DIKeyState(DIK_W))
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_E);
		else if (m_pGameInstance->Get_DIKeyState(DIK_S))
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_E);
		else if (m_pGameInstance->Get_DIKeyState(DIK_A))
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_E);
		else if (m_pGameInstance->Get_DIKeyState(DIK_D))
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_E);
		else
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD);
				//TODO::24042219 일단 바로 장전 모션 나오게 제작
			//만약 탄창 빈공간 이 있으면 재장전 시작하는 조건문(번호 받아오기)
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

void CState_Reload_S::Exit(CPlayer* Player)
{
}

CState* CState_Reload_S::Create()
{
	CState_Reload_S* pInstance = new CState_Reload_S();
	return pInstance;
}

void CState_Reload_S::Free()
{
	__super::Free();
}
