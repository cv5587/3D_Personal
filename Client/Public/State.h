#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CState :
    public CBase
{
protected:
    CState();
    virtual ~CState() = default;

public:
   virtual void Enter(class CPlayer* Player)=0;
   virtual void Update(class CPlayer* Player, _float fTimeDelta) =0;
   virtual void Exit(class CPlayer* Player) =0 ;

protected:
    class CGameInstance* m_pGameInstance = { nullptr };

public:
    virtual void Free() override;
};

END