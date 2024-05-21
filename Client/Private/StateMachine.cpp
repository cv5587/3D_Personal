#include "StateMachine.h"
#include "Player.h"
#include "State.h"

#pragma region"상태 정의"
#include "State_Idle.h"
#include "State_Exhausted.h"
#include "State_Jog.h"
#include "State_Run.h"
#include "State_Equip.h"
#include "State_UnEquip.h"
#include "State_Aim_S.h"
#include "State_Aim.h"
#include "State_Aim_E.h"
#include "State_Walk_S.h"
#include "State_Walk_FB.h"
#include "State_Walk_L.h"
#include "State_Walk_R.h"
#include "State_Walk_LE.h"
#include "State_Walk_E.h"
#include "State_Walk_RE.h"
#include "State_Throw.h"
#include "State_Attack.h"
#include "State_Fire.h"
#include "State_Hipfire.h"
#include "State_Reload_S.h"
#include "State_Reload.h"
#include "State_Enter.h"
#include "State_Reload_E.h"
#include "State_PickUp.h"
#include "State_Inventory.h"
#include "State_Kill.h"
#include "State_Release.h"
#pragma endregion

IMPLEMENT_SINGLETON(CStateMachine)

CStateMachine::CStateMachine()
{
}

HRESULT CStateMachine::Initialize()
{
	CState* pState = nullptr;

	 pState = CState_Idle::Create();
	m_vecStates.push_back(pState);

	pState = CState_Exhausted::Create();
	m_vecStates.push_back(pState);

	pState = CState_Jog::Create();
	m_vecStates.push_back(pState);

	pState = CState_Run::Create();
	m_vecStates.push_back(pState);

	pState = CState_Equip::Create();
	m_vecStates.push_back(pState);

	pState = CState_UnEquip::Create();
	m_vecStates.push_back(pState);

	pState = CState_Aim_S::Create();
	m_vecStates.push_back(pState);

	pState = CState_Aim::Create();
	m_vecStates.push_back(pState);

	pState = CState_Aim_E::Create();
	m_vecStates.push_back(pState);

	pState = CState_Walk_S::Create();
	m_vecStates.push_back(pState);

	pState = CState_Walk_E::Create();
	m_vecStates.push_back(pState);

	pState = CState_Walk_FB::Create();
	m_vecStates.push_back(pState);

	pState = CState_Walk_L::Create();
	m_vecStates.push_back(pState);

	pState = CState_Walk_R::Create();
	m_vecStates.push_back(pState);

	pState = CState_Walk_LE::Create();
	m_vecStates.push_back(pState);

	pState = CState_Walk_RE::Create();
	m_vecStates.push_back(pState);

	pState = CState_Throw::Create();
	m_vecStates.push_back(pState);

	pState = CState_Attack::Create();
	m_vecStates.push_back(pState);

	pState = CState_Fire::Create();
	m_vecStates.push_back(pState);

	pState = CState_Hipfire::Create();
	m_vecStates.push_back(pState);

	pState = CState_Reload_S::Create();
	m_vecStates.push_back(pState);

	pState = CState_Reload::Create();
	m_vecStates.push_back(pState);

	pState = CState_Enter::Create();
	m_vecStates.push_back(pState);

	pState = CState_Reload_E::Create();
	m_vecStates.push_back(pState);

	pState = CState_PickUp::Create();
	m_vecStates.push_back(pState);

	pState = CState_Inventory::Create();	
	m_vecStates.push_back(pState);
	
	pState = CState_Kill::Create();
	m_vecStates.push_back(pState);

	pState = CState_Release::Create();
	m_vecStates.push_back(pState);

	//m_eCurrentState = m_vecStates[(_uint)PLAYERSTATE::PLAYER_IDLE];

	return S_OK;
}

void CStateMachine::Set_CurrentState(class CPlayer* Player, PLAYERSTATE eNewState)
{
	if (nullptr!= m_eCurrentState)
	{
		m_eCurrentState->Exit(Player);
	}

	m_eCurrentState = m_vecStates[(_uint)eNewState];

	if (nullptr != m_eCurrentState)
	{
		m_eCurrentState->Enter(Player);
	}
}

void CStateMachine::Update(class CPlayer* Player, _float fTimeDelta)
{
	if (nullptr != m_eCurrentState)
	{
		m_eCurrentState->Update(Player, fTimeDelta);
	}
}

void CStateMachine::Free()
{
	Safe_Release(m_eCurrentState);

	for (auto& iter : m_vecStates)
		Safe_Release(iter);
	m_vecStates.clear();
}
