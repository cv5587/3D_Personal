#include "State_Enter.h"
#include "Player.h"

CState_Enter::CState_Enter()
	:CState{}
{
}

void CState_Enter::Enter(CPlayer* Player)
{
}

void CState_Enter::Update(CPlayer* Player, _float fTimeDelta)
{
	Player->Loading_UI(fTimeDelta);

	if (!Player->isEnter())
	{
		Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
		return;
	}

	if (m_pGameInstance->Get_DIMouseState(DIM_LB))
	{
		Player->Pick_up();
	
		Player->Add_EnterTime(fTimeDelta);	
		return;
	}

	Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
	return;

	Player->Mouse_Fix();
}

void CState_Enter::Exit(CPlayer* Player)
{
	Player->Set_Enter(false);
}

CState* CState_Enter::Create()
{
	CState_Enter* pInstance = new CState_Enter();
	return pInstance;
}

void CState_Enter::Free()
{
	__super::Free();
}
