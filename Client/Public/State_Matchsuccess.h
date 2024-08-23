#pragma once
#include "State.h"
BEGIN(Client)
class CState_Matchsuccess :
    public CState
{
protected:
    CState_Matchsuccess();
    virtual ~CState_Matchsuccess() = default;

public:
    virtual void Enter(class CPlayer* Player) override;
    virtual void Update(class CPlayer* Player, _float fTimeDelta)override;
    virtual void Exit(class CPlayer* Player)override;

    _float m_fCurrentTime = { 0.f };
public:
    static CState* Create();
    virtual void Free() override;
};
END
