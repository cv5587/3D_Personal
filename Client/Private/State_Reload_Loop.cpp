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

	//���� ��Ŀ���� ���� �ؼ� �־��ֱ� 
	//�ִϸ��̼� ���� ��ŭ Ŭ���� ���� or ���� �޾ƿͼ� �ݺ� ��� ����
	if (Player->isAnimFinished()&&Player->isRevolver_AnimFin())//
	{
		//Player->End_Change();
		//�ִϸ��̼� ���� �ѹ��� ���������� �����Ǵ� �Ѿ˰��� ���� 
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
				// ���� Ƚ���� ��� ä��� ���� ������.

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
