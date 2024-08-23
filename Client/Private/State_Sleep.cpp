#include "State_Sleep.h"
#include "Player.h"
CState_Sleep::CState_Sleep()
	:CState{}
{
}

void CState_Sleep::Enter(CPlayer* Player)
{
}

void CState_Sleep::Update(CPlayer* Player, _float fTimeDelta)
{
	Player->UISleep_Update(fTimeDelta);

	if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
	{
		Player->Pick_UISleep();
	}

}

void CState_Sleep::Exit(CPlayer* Player)
{
	m_pGameInstance->Play_Sound(TEXT("BirdMorning.wav"), ENTER, 1.0f);
}

CState* CState_Sleep::Create()
{
	CState_Sleep* pInstance = new CState_Sleep();
	return pInstance;
}

void CState_Sleep::Free()
{
	__super::Free();
}
