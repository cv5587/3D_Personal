#include "State_Inventory.h"
#include "Player.h"
CState_Inventory::CState_Inventory()
	:CState{}
{
}

void CState_Inventory::Enter(CPlayer* Player)
{
	Player->Set_SceneSelect(2);
	m_pGameInstance->Play_Sound(TEXT("OpenInven.wav"), INVENTORY, 1.0f);
}

void CState_Inventory::Update(CPlayer* Player, _float fTimeDelta)
{
	Player->Inventory_Update(fTimeDelta);

	if (m_pGameInstance->Get_DIMouseState_Once(DIM_LB))
	{
		Player->Pick_UI();
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

void CState_Inventory::Exit(CPlayer* Player)
{
	m_pGameInstance->Play_Sound(TEXT("OpenInven.wav"), INVENTORY, 1.0f);
}

CState* CState_Inventory::Create()
{
	CState_Inventory* pInstance = new CState_Inventory();
	return pInstance;
}

void CState_Inventory::Free()
{
	__super::Free();
}
