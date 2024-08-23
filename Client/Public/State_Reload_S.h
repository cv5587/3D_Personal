#pragma once
#include "State.h"
BEGIN(Client)
class CState_Reload_S :
    public CState
{
protected:
    CState_Reload_S();
    virtual ~CState_Reload_S() = default;

public:
    virtual void Enter(class CPlayer* Player) override;
    virtual void Update(class CPlayer* Player, _float fTimeDelta)override;
    virtual void Exit(class CPlayer* Player)override;

private:
    _float m_fCurrentTime = {0.f};
    _bool   m_bOnce = { false };
public:
    static CState* Create();
    virtual void Free() override;
};

END