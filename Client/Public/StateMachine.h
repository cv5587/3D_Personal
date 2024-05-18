#pragma once
#include "Client_Defines.h"
#include "Base.h"


BEGIN(Client)
class CStateMachine final:
    public CBase
{
    DECLARE_SINGLETON(CStateMachine)
private:
    CStateMachine();
    virtual ~CStateMachine() = default;

public:
    HRESULT Initialize();
    void Set_CurrentState(class CPlayer* Player,PLAYERSTATE eNewState);
    void Update(class CPlayer* Player, _float fTimeDelta);

private:
    class CState* m_eCurrentState = { nullptr };
    vector<class CState*> m_vecStates;

public:
    virtual void Free() override;
};

END