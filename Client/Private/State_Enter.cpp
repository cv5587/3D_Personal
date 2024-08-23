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
		m_pGameInstance->Play_Sound(TEXT("DoorEnter.wav"), ENTER, 0.3f);
		Player->Pick_InteractiveObject();
	
		Player->Add_EnterTime(fTimeDelta);	
		return;
	}

	Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
	return;

	Player->Mouse_Fix();
}

void CState_Enter::Exit(CPlayer* Player)
{
	m_bIn = !m_bIn;
	if(m_bIn)
	{
		m_pGameInstance->Play_BGM(TEXT("InSnow.wav"), 0.6f);
	}
	else
	{
		m_pGameInstance->Play_BGM(TEXT("OutSnow.wav"), 0.6f);
	}

	m_pGameInstance->Get_DIMouseState_Once(DIM_LB);
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
