#include "State_PickUp.h"
#include "Player.h"
CState_PickUp::CState_PickUp()
	:CState{}
{
}

void CState_PickUp::Enter(CPlayer* Player)
{
	
}

void CState_PickUp::Update(CPlayer* Player, _float fTimeDelta)
{
	Player->Pick_up_Update(fTimeDelta);

	if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
	{
			Player->Add_Item();
			if (CON_NORMAL == Player->isCondition())
			{
				Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
			}
			else
			{
				Player->Set_State(PLAYERSTATE::PLAYER_IDLE_EXHAUSTED);
			}
			Player->Mouse_Fix();
			return;
	}
	
	if (m_pGameInstance->Get_DIMouseState_Once(DIM_RB))
	{
		Player->Drop_Item();
		if (CON_NORMAL == Player->isCondition())
		{
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
		}
		else
		{
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE_EXHAUSTED);
		}
		Player->Mouse_Fix();
		return;
	}

	Player->Mouse_Fix();
}

void CState_PickUp::Exit(CPlayer* Player)
{
}

CState* CState_PickUp::Create()
{
	CState_PickUp* pInstance = new CState_PickUp();
	return pInstance;
}

void CState_PickUp::Free()
{
	__super::Free();
}
