#pragma once
#include "State.h"
BEGIN(Client)
class CState_Fire final:
    public CState
{
protected:
    CState_Fire();
    virtual ~CState_Fire() = default;

public:
    virtual void Enter(class CPlayer* Player) override;
    virtual void Update(class CPlayer* Player, _float fTimeDelta)override;
    virtual void Exit(class CPlayer* Player)override;
private:
    _bool isbFire = { false };
public:
    static CState* Create();
    virtual void Free() override;
};
END
