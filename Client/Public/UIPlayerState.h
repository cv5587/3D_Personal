#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "UImanager.h"
BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Client)
class CUIPlayerState :
    public CBase
{

public:
    typedef struct UISTATE_DESC {
        class CNPCManager* pNPCManager;
    }UISTATE_DESC;

private:
    CUIPlayerState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CUIPlayerState() = default;
public:
    HRESULT Initialize(void* pArg);
    HRESULT Tick(_float fTimeDelta);
    HRESULT Late_Tick(_float fTimeDelta);

private:
    HRESULT Make_UI();


private:
    vector<wstring> m_UISelector;

    class CGameInstance* m_pGameInstance = { nullptr };
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    //레이어 이름을 가지고 있음
    class CNPCManager* m_pNPCManager = { nullptr };

    vector<wstring> m_wLayerName;
    vector<class CGameObject*> m_pText;//0번레이어 버튼


    _float m_fX, m_fY;
public:
    static CUIPlayerState* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual void Free() override;
};
END

