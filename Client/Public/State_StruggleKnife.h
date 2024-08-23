#pragma once
#include "State.h"
BEGIN(Client)
class CState_StruggleKnife final :
    public CState
{
protected:
    CState_StruggleKnife();
    virtual ~CState_StruggleKnife() = default;

public:
    virtual void Enter(class CPlayer* Player) override;
    virtual void Update(class CPlayer* Player, _float fTimeDelta)override;
    virtual void Exit(class CPlayer* Player)override;
private:
    _float m_fTime = { 0.f };
    _float m_fTwoTime = { 0.f };
    _bool m_bOnce = { false };
    _bool m_bTwoOnce = { false };
public:
    static CState* Create();
    virtual void Free() override;
};

END