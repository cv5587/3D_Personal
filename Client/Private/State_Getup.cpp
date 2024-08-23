#include "State_Getup.h"
#include "Player.h"
CState_Getup::CState_Getup()
	:CState()
{
}

void CState_Getup::Enter(CPlayer* Player)
{
}

void CState_Getup::Update(CPlayer* Player, _float fTimeDelta)
{
	if (Player->isAnimFinished())
	{
		Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
	}
}

void CState_Getup::Exit(CPlayer* Player)
{

}

CState* CState_Getup::Create()
{
	CState_Getup* pInstance = new CState_Getup();
	return pInstance;
}

void CState_Getup::Free()
{
	__super::Free();
}