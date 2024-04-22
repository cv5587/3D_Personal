#include "State_Reload.h"
#include "Player.h"

CState_Reload::CState_Reload()
	:CState{}
{
}

void CState_Reload::Enter(CPlayer* Player)
{
}

void CState_Reload::Update(CPlayer* Player, _float fTimeDelta)
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
		//�ִϸ��̼� ���� �ѹ��� ���������� �����Ǵ� �Ѿ˰��� ���� 
		Player->End_Change();

		if (m_pGameInstance->Get_DIMouseState(DIM_LB))
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_E);
		else
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_E);
				// ���� Ƚ���� ��� ä��� ���� ������.
	}
}

void CState_Reload::Exit(CPlayer* Player)
{
}

CState* CState_Reload::Create()
{
	CState_Reload* pInstance = new CState_Reload();
	return pInstance;
}

void CState_Reload::Free()
{
	__super::Free();
}
