#pragma once
#include "State.h"
BEGIN(Client)
class CState_Reload final:
    public CState
{
protected:
    CState_Reload();
    virtual ~CState_Reload() = default;

public:
    virtual void Enter(class CPlayer* Player) override;
    virtual void Update(class CPlayer* Player, _float fTimeDelta)override;
    virtual void Exit(class CPlayer* Player)override;
    _bool m_AddBullet = { false };
    _float m_CurrentTime = { 0.f };
public:
    static CState* Create();
    virtual void Free() override;
};

END