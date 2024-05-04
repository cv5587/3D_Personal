#pragma once
#include "Client_Defines.h"
#include "Base.h"
BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CInventory;
END

BEGIN(Client)
class CPickUpSelector :
    public CBase
{
    typedef struct {
        //_float fSensor;
        
    }SELECTOR_DESC;
private:
    CPickUpSelector(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CPickUpSelector() = default;

private:
    class CGameInstance* m_pGameInstance = { nullptr };
    class ID3D11Device* m_pDevice;
    class ID3D11DeviceContext* m_pContext;
public:
    HRESULT Initialize(void* pArg=nullptr);
    HRESULT Pick_up(class CGameObject* pPickObject, _bool* Acquire);
    HRESULT Tick(_float fTimeDelta);
    HRESULT Late_Tick(_float fTimeDelta);
    HRESULT Render();

private:
    _bool* m_pAcquire = { false };
    class CGameObject* m_PickObject = { nullptr };
    _float4x4 m_PickObjectOriginMatrix = {};
    _float m_fSensor = { 0.2f };

    class CInventory* m_pPlayerInventory = { nullptr };
    class CUImanager* m_pUImanager = { nullptr };

    _bool m_UIInfoSet = { false };

public:
    static CPickUpSelector* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg=nullptr);
    virtual void Free() override;
};

END