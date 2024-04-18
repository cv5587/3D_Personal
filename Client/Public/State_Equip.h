#pragma once
#include "State.h"
BEGIN(Client)
class CState_Equip :
    public CState
{
protected:
    CState_Equip();
    virtual ~CState_Equip() = default;

public:
    virtual void Enter(class CPlayer* Player) override;
    virtual void Update(class CPlayer* Player, _float fTimeDelta)override;
    virtual void Exit(class CPlayer* Player)override;

public:
    static CState* Create();
    virtual void Free() override;
};

END