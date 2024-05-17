#pragma once
#include "State.h"
BEGIN(Client)
class CState_Throw final:
    public CState
{
protected:
    CState_Throw();
    virtual ~CState_Throw() = default;

public:
    virtual void Enter(class CPlayer* Player) override;
    virtual void Update(class CPlayer* Player, _float fTimeDelta)override;
    virtual void Exit(class CPlayer* Player)override;

private:
    _float m_fThrowTime = { 0.f };
    _bool m_bShot = { false };
public:
    static CState* Create();
    virtual void Free() override;
};
END
