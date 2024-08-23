#include "State_Quest.h"
#include "Player.h"

CState_Quest::CState_Quest()
	:CState{}
{
}

void CState_Quest::Enter(CPlayer* Player)
{
}

void CState_Quest::Update(CPlayer* Player, _float fTimeDelta)
{
	if (Player->Talk(m_iTalkIndex))
	{
		Player->Quest_Update(fTimeDelta);
		if (m_pGameInstance->Get_DIKeyState_Once(DIK_F))
		{
			m_iTalkIndex=++m_iTalkIndex;
		}

		if (m_pGameInstance->Get_DIKeyState_Once(DIK_ESCAPE))
		{
			Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
		}
	}
	else
	{
		Player->Set_State(PLAYERSTATE::PLAYER_IDLE);
	}
	
	Player->Mouse_Fix();
}

void CState_Quest::Exit(CPlayer* Player)
{
	m_iTalkIndex = 0;
}

CState* CState_Quest::Create()
{

	CState_Quest* pInstance = new CState_Quest();
	return pInstance;
}

void CState_Quest::Free()
{
	__super::Free();
}
