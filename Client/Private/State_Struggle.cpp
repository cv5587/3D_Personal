#include "State_Struggle.h"
#include "Player.h"
CState_Struggle::CState_Struggle()
    :CState{}
{
}

void CState_Struggle::Enter(CPlayer* Player)
{
	Player->Set_UnEquip();
	m_pGameInstance->StopSound(CH_WOLF);

}

void CState_Struggle::Update(CPlayer* Player, _float fTimeDelta)
{

	if (!m_bOnce)
	{
		m_pGameInstance->StopSound(CH_WOLF);
		m_pGameInstance->Play_Sound(TEXT("WolfStruggle.wav"), CH_WOLF, 1.f);
		m_bOnce = true;
	}
	Player->UIStruggle_Update(fTimeDelta);
	Player->Add_EnterTime(fTimeDelta);
	if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
	{
		Player->Pick_UIStruggle();
	}


	if (Player->isAnimFinished())
	{
		if (Player->isStruggleItem())
		{
			Player->Set_NoActionEquip(EQUIP_KNIFE);
			Player->Set_State(PLAYERSTATE::PLAYER_STRUGGLEKNIFE);
		}
		else
		{
			Player->Set_State(PLAYERSTATE::PLAYER_STRUGGLEBARE);
		}
	}

}

void CState_Struggle::Exit(CPlayer* Player)
{
	m_bOnce = false;
	Player->CurrentLoadingBarReset();
}

CState* CState_Struggle::Create()
{
	CState_Struggle* pInstance = new CState_Struggle();
	return pInstance;
}

void CState_Struggle::Free()
{
	__super::Free();
}
