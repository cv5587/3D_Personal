#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "UImanager.h"
BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CInventory;
class CItemData;
END

BEGIN(Client)
class CUIStruggleIntro :
    public CBase
{

public:
    typedef struct STRUGGLEINTRO_DESC {
        _float* pCurrentBar;
        CInventory* pPlayerInventory;
        CUImanager* pUImanager;
    }STRUGGLEINTRO_DESC;

private:
    CUIStruggleIntro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CUIStruggleIntro() = default;
public:
    HRESULT Initialize(void* pArg);
    HRESULT Tick(_float fTimeDelta);
    HRESULT Late_Tick(_float fTimeDelta);

    HRESULT Pick_UIStruggle();

private:
    HRESULT Make_UI();
    HRESULT Make_UILoad();

private:
    class CGameInstance* m_pGameInstance = { nullptr };
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    class CInventory* m_pPlayerInventory = { nullptr };
    class CUImanager* m_pUImanager = { nullptr };

    vector<wstring> m_wLayerName;
    vector<class CGameObject*> m_pButton;//2번맨손 0,1번 무기(있으면 띄우기.)
    vector<class CGameObject*> m_pLoading;
    _float m_fX, m_fY;

    _bool m_bisItem = { false };
public:
    static CUIStruggleIntro* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual void Free() override;
};
END
