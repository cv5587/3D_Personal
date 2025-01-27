#include "State_Kill.h"
#include "Player.h"

CState_Kill::CState_Kill()
	:CState{}
{
}

void CState_Kill::Enter(CPlayer* Player)
{
}

void CState_Kill::Update(CPlayer* Player, _float fTimeDelta)
{
	if(!m_bOnce)
	{
		m_pGameInstance->Play_Sound(TEXT("RabbitKill.wav"), CH_RABBIT, 1.f);
		m_bOnce = true;
	}
	if (Player->isAnimFinished())
	{
		Player->Add_Rabbit();
	
		if (CON_NORMAL == Player->isCondition())
		{
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
		}
		else
		{
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE_EXHAUSTED);
		}
		
		Player->Set_UnEquip();
		Player->End_Change();
	}
	Player->Mouse_Fix();
}

void CState_Kill::Exit(CPlayer* Player)
{
	m_bOnce = false;
}

CState* CState_Kill::Create()
{
	CState_Kill* pInstance = new CState_Kill();
	return pInstance;
}

void CState_Kill::Free()
{
	__super::Free();
}
