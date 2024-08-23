#include "State_FlareSuccess.h"
#include "Player.h"
CState_FlareSuccess::CState_FlareSuccess()
	:CState{}
{
}

void CState_FlareSuccess::Enter(CPlayer* Player)
{
}

void CState_FlareSuccess::Update(CPlayer* Player, _float fTimeDelta)
{
	m_fCurrentTime += fTimeDelta;

	if (1.2f < m_fCurrentTime)
	{
		Player->Set_FlareLit(true);
	}
	if (1.2f < m_fCurrentTime)
	{
		m_pGameInstance->Set_LightSwitch(4, true);
	}

	if (Player->isAnimFinished())
	{
		Player->Using_Item(TEXT("Flare"));
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
			if (CON_NORMAL == Player->isCondition())
			{
				Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
			}
			else
			{
				Player->Set_State(PLAYERSTATE::PLAYER_IDLE_EXHAUSTED);
			}
		}

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

void CState_FlareSuccess::Exit(CPlayer* Player)
{
	m_fCurrentTime = 0.f;
}

CState* CState_FlareSuccess::Create()
{
	CState_FlareSuccess* pInstance = new CState_FlareSuccess();
	return pInstance;
}

void CState_FlareSuccess::Free()
{
	__super::Free();
}
