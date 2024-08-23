#pragma once
#include "State.h"
BEGIN(Client)
class CState_FlareSuccess :
    public CState
{
protected:
    CState_FlareSuccess();
    virtual ~CState_FlareSuccess() = default;

public:
    virtual void Enter(class CPlayer* Player) override;
    virtual void Update(class CPlayer* Player, _float fTimeDelta)override;
    virtual void Exit(class CPlayer* Player)override;

private:
    _float m_fCurrentTime = { 0.f };
public:
    static CState* Create();
    virtual void Free() override;
};
END