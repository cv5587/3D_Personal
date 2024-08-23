#include "State_Run.h"
#include "Player.h"
CState_Run::CState_Run()
	:CState{}
{
}

void CState_Run::Enter(CPlayer* Player)
{
	if (Player->isCondition() == CON_NORMAL)
		Player->Set_DeltaValue(SPEED_RUN, XMConvertToRadians(120.0f));
	else
		Player->Set_DeltaValue(SPEED_EX_RUN, XMConvertToRadians(120.0f));
}

void CState_Run::Update(CPlayer* Player, _float fTimeDelta)
{

	m_fFootTime += fTimeDelta;
	if (m_fFootTime > 0.2f)
	{
		m_pGameInstance->StopSound(CH_FOOT);
		m_pGameInstance->Play_Sound(TEXT("Foot.wav"), CH_FOOT, 0.5f);
		m_fFootTime = 0.f;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_W))
	{
		Player->Set_State(PLAYERSTATE::PLAYER_JOG);
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
		Player->Set_State(PLAYERSTATE::PLAYER_JOG);
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
		Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		Player->Go_Left(fTimeDelta);
	}
	else 	if (m_pGameInstance->Get_DIKeyState(DIK_D))
	{
		Player->Set_State(PLAYERSTATE::PLAYER_JOG);
		Player->Go_Right(fTimeDelta);
	}
	else if (m_pGameInstance->Get_DIMouseState(DIM_RB))
	{
		if (Player->isEquip() != PLAYEREQUIP::EQUIP_NONE)
			Player->Set_State(PLAYERSTATE::PLAYER_AIM_S);
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_R))
		if (Player->isEquip() == PLAYEREQUIP::EQUIP_REVOLVER)
			Player->Set_State(PLAYERSTATE::PLAYER_RELOAD_S);
	else
	{
		Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
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



	if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT)&& m_pGameInstance->Get_DIKeyState(DIK_W))
	{
		Player->Set_State(PLAYERSTATE::PLAYER_RUN);
		Player->Go_Straight(fTimeDelta);
	}

	Player->Mouse_Fix();
}

void CState_Run::Exit(CPlayer* Player)
{
	m_fFootTime = 0.f;
}

CState* CState_Run::Create()
{
	CState_Run* pInstance = new CState_Run();
	return pInstance;
}

void CState_Run::Free()
{
	__super::Free();
}
