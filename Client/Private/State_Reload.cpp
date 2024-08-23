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
	if(!m_AddBullet)
	{
		m_CurrentTime += fTimeDelta;
		if(m_CurrentTime>0.6f)
		{
			m_pGameInstance->StopSound(CH_GUN);
			m_pGameInstance->Play_Sound(TEXT("Reloading.wav"), CH_GUN, 0.5f);
			Player->isReload();
			m_AddBullet = true;
		}
	}
	if (Player->isAnimFinished()&&Player->isRevolver_AnimFin())//
	{

		//Player->End_Change();
		//�ִϸ��̼� ���� �ѹ��� ���������� �����Ǵ� �Ѿ˰��� ���� 
		//if (!m_AddBullet)
		//{
		//	Player->isReload();
		//	m_AddBullet = true;
		//}
		if (true==m_AddBullet &&6 != Player->Get_BullletLeft())
		{
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD);	
			Player->Set_Reload_Reset();
			
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

void CState_Reload::Exit(CPlayer* Player)
{
	m_AddBullet = false;
	m_CurrentTime = { 0.f };
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
