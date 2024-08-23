#include "State_Burn.h"
#include "Player.h"
CState_Burn::CState_Burn()
	:CState{}
{
}

void CState_Burn::Enter(CPlayer* Player)
{
}

void CState_Burn::Update(CPlayer* Player, _float fTimeDelta)
{
	Player->UIBurn_Update(fTimeDelta);
	if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
	{
		Player->Pick_UIBurn();
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

void CState_Burn::Exit(CPlayer* Player)
{
	
}

CState* CState_Burn::Create()
{

	CState_Burn* pInstance = new CState_Burn();
	return pInstance;
}

void CState_Burn::Free()
{
	__super::Free();
}
