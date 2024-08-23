#include "State_Matchsuccess.h"
#include "Player.h"
CState_Matchsuccess::CState_Matchsuccess()
	:CState{}
{
}

void CState_Matchsuccess::Enter(CPlayer* Player)
{
	
}

void CState_Matchsuccess::Update(CPlayer* Player, _float fTimeDelta)
{
	m_fCurrentTime += fTimeDelta;

	if (0.2f < m_fCurrentTime)
	{
		Player->Set_Lit(true);
		
	}
	if(0.4f<m_fCurrentTime)
	{
		m_pGameInstance->Set_LightSwitch(2, true);
		m_pGameInstance->Play_Sound(TEXT("MatchStart.wav"), MATCH, 0.8f);
	}

	if (Player->isAnimFinished())
	{
		m_pGameInstance->StopSound(MATCH);
		m_pGameInstance->Play_Sound(TEXT("FireLoop.wav"), MATCH, 1.0f);
		Player->Using_Item(TEXT("Matchbox"));
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

void CState_Matchsuccess::Exit(CPlayer* Player)
{
	m_fCurrentTime = 0.f;
}

CState* CState_Matchsuccess::Create()
{
	CState_Matchsuccess* pInstance = new CState_Matchsuccess();
	return pInstance;
}

void CState_Matchsuccess::Free()
{
	__super::Free();
}
