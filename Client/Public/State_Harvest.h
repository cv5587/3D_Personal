#pragma once
#include "State.h"
BEGIN(Client)
class CState_Harvest :
    public CState
{
protected:
    CState_Harvest();
    virtual ~CState_Harvest() = default;

public:
    virtual void Enter(class CPlayer* Player) override;
    virtual void Update(class CPlayer* Player, _float fTimeDelta)override;
    virtual void Exit(class CPlayer* Player)override;


public:
    static CState* Create();
    virtual void Free() override;
};

END