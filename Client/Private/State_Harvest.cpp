#include "State_Harvest.h"
#include "Player.h"
CState_Harvest::CState_Harvest()
	:CState{}
{
}

void CState_Harvest::Enter(CPlayer* Player)
{
}

void CState_Harvest::Update(CPlayer* Player, _float fTimeDelta)
{
	Player->UIHarvest_Update(fTimeDelta);
	if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
	{
		Player->Pick_UIHarvest();
	}

	if (m_pGameInstance->Get_DIKeyState_Once(DIK_ESCAPE))
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
}

void CState_Harvest::Exit(CPlayer* Player)
{
	Player->CurrentLoadingBarReset();
}

CState* CState_Harvest::Create()
{

	CState_Harvest* pInstance = new CState_Harvest();
	return pInstance;
}

void CState_Harvest::Free()
{
	__super::Free();
}
