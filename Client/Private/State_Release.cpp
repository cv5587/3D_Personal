#include "State_Release.h"
#include "Player.h"
CState_Release::CState_Release()
	:CState{}
{
}

void CState_Release::Enter(CPlayer* Player)
{
}

void CState_Release::Update(CPlayer* Player, _float fTimeDelta)
{
	if (Player->isAnimFinished())
	{
		

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

void CState_Release::Exit(CPlayer* Player)
{
}

CState* CState_Release::Create()
{
	CState_Release* pInstance = new CState_Release();
	return pInstance;
}

void CState_Release::Free()
{
    __super::Free();
}
