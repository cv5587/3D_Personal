#include "StateMachine.h"
#include "Player.h"
#include "State.h"
#include "State_Idle.h"
#include "State_Exhausted.h"
#include "State_Jog.h"
#include "State_Run.h"
#include "State_Equip.h"
#include "State_UnEquip.h"

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


	//m_eCurrentState = m_vecStates[(_uint)PLAYERSTATE::PLAYER_IDLE];

	return S_OK;
}

void CStateMachine::Set_CurrentState(class CPlayer* Player, PLAYERSTATE eNewState)
{
	if (nullptr!= m_eCurrentState)
	{
		m_eCurrentState->Exit(Player);
	}

	//Safe_Release(m_eCurrentState);
	m_eCurrentState = m_vecStates[(_uint)eNewState];
	//Safe_AddRef(m_eCurrentState);

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
