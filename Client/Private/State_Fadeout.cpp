#include "State_Fadeout.h"
#include "Player.h"

CState_Fadeout::CState_Fadeout()
	:CState{}
{
}

void CState_Fadeout::Enter(CPlayer* Player)
{
}

void CState_Fadeout::Update(CPlayer* Player, _float fTimeDelta)
{
	Player->Set_FadeIn();
	Player->Add_EnterTime(fTimeDelta);
	if (Player->isLoadFinished())
	{
		Player->CurrentLoadingBarReset();
		Player->Set_State(PLAYER_IDLE);
	}
}

void CState_Fadeout::Exit(CPlayer* Player)
{

}

CState* CState_Fadeout::Create()
{
	CState_Fadeout* pInstance = new CState_Fadeout();
	return pInstance;
}

void CState_Fadeout::Free()
{
	__super::Free();
}
