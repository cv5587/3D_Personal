#pragma once
#include "State.h"
BEGIN(Client)
class CState_Struggle final :
    public CState
{
protected:
    CState_Struggle();
    virtual ~CState_Struggle() = default;

public:
    virtual void Enter(class CPlayer* Player) override;
    virtual void Update(class CPlayer* Player, _float fTimeDelta)override;
    virtual void Exit(class CPlayer* Player)override;

private:
    _bool m_bItem = { false };
    _bool m_bOnce = { false };
public:
    static CState* Create();
    virtual void Free() override;
};

END