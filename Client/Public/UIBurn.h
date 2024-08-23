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
class CUIBurn :
    public CBase
{
    enum STOVE{ STOVE_BURN, STOVE_LOAD , STOVE_SELECT, STOVE_FUEL , STOVE_COOK , STOVE_BOIL , STOVE_END};
public:
    typedef struct UIBURN_DESC{
        CInventory* pPlayerInventory;
        CUImanager* pUImanager;
    }UIBURN_DESC;

private:
    CUIBurn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CUIBurn() = default;
public:
    HRESULT Initialize(void* pArg);
    HRESULT Tick(_float fTimeDelta);
    HRESULT Late_Tick(_float fTimeDelta);




public:
    void Set_Stove(class CGameObject* PickObject);
    HRESULT Pick_UIBurn();

private:
    HRESULT Make_UI();
    HRESULT Make_Load();
    HRESULT Make_SelectUI();
    HRESULT Make_FuelUI();
    HRESULT Make_CookedUI();
    HRESULT Make_BoilUI();

private:
    class CGameInstance* m_pGameInstance = { nullptr };
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    //레이어 이름을 가지고 있음
    class CInventory* m_pPlayerInventory = { nullptr };
    class CUImanager* m_pUImanager = { nullptr };
    class CStove* m_pStove = { nullptr };
    
    vector<wstring> m_wLayerName;
    vector<class CGameObject*> m_pButton;//0번레이어 버튼
    class CGameObject* m_pName = { nullptr };
    vector<class CGameObject*> m_pIcon;
    vector<class CGameObject*> m_pSelector;
    vector<class CGameObject*>m_pFood;

    //연료추가
    vector<class CGameObject*> m_pFuelButton;//0번레이어 버튼
    vector<class CGameObject*> m_pFuelIcon;
    //로드
    vector<class CGameObject*> m_pLoading;

   
    class CGameObject* m_pTimeUI = { nullptr };

    _float m_fX, m_fY;
    _float m_Time = { 0.f };

    _uint m_iState = { STOVE_BURN };
    
public:
    static CUIBurn* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual void Free() override;
};
END

