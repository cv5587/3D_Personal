#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "UImanager.h"
BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Client)
class CUISleep :
    public CBase
{
public:
    typedef struct UISLEEP_DESC {

        CUImanager* pUImanager;
    }UISLEEP_DESC;
    enum SLEEP{SLEEP_SELECT, SLEEP_LOAD ,SLEEP_END};
private:
    CUISleep(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CUISleep() = default;
public:
    HRESULT Initialize(void* pArg);
    HRESULT Tick(_float fTimeDelta);
    HRESULT Late_Tick(_float fTimeDelta);
    HRESULT Pick_UISleep();
private:
    HRESULT Make_UI();

public:
    void Set_State(_uint Index) { m_iState = Index; }
private:
    class CGameInstance* m_pGameInstance = { nullptr };
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    //레이어 이름을 가지고 있음
    class CUImanager* m_pUImanager = { nullptr };

    vector<wstring> m_wLayerName;
    vector<class CGameObject*> m_pButton;//잠자기,증가감소
    vector<class CGameObject*> m_pText; // 변경 하는 텍스트 시간1개
    _uint m_iState = { SLEEP_SELECT };
    _uint m_iTime = { 1 };
    _float m_fX, m_fY;

public:
    static CUISleep* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual void Free() override;
};

END