#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
END
BEGIN(Client)
class CLoadingBar :
    public CBase
{
public:
    typedef struct {
        CUImanager* pUImanager;
        _float* pCurrentLoadingBar;
    }LOADINGBAR_DESC;

private:
    CLoadingBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CLoadingBar() = default;

private:
    class CGameInstance* m_pGameInstance = { nullptr };
     ID3D11Device* m_pDevice;
     ID3D11DeviceContext* m_pContext;
public:
    HRESULT Initialize(void* pArg );
    HRESULT Tick(_float fTimeDelta);
    HRESULT Render();


private:
    _float* m_pCurrentLoadingBar = { nullptr };
    class CUImanager* m_pUImanager = { nullptr };


public:
    static CLoadingBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);
    virtual void Free() override;
};
END
