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
class CHarvest :
    public CBase
{
public:
    typedef struct UIHARVEST_DESC {
        CInventory* pPlayerInventory;
        CUImanager* pUImanager;
    }UIHARVEST_DESC;
    enum HARVEST {HARVEST_SELECT, HARVEST_LOAD ,HARVEST_END};
private:
    CHarvest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CHarvest() = default;
public:
    HRESULT Initialize(void* pArg);
    HRESULT Tick(_float fTimeDelta);
    HRESULT Late_Tick(_float fTimeDelta);
    HRESULT Pick_UIHarvest();
private:
    HRESULT Make_UI();


private:
    class CGameInstance* m_pGameInstance = { nullptr };
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    //���̾� �̸��� ������ ����
    class CInventory* m_pPlayerInventory = { nullptr };
    class CUImanager* m_pUImanager = { nullptr };

    vector<wstring> m_wLayerName;
    vector<class CGameObject*> m_pButton;//0�����̾� ��ư
    vector<class CGameObject*> m_pNumText;
    vector<class CGameObject*> m_pInfo;
    vector<class CGameObject*> m_pText; // ���� �ϴ� �ؽ�ó
    _float m_fX, m_fY;

    _uint m_iTime = { 0 }; //�ҿ�ð�
    _uint m_iNumMeat = { 0 };//���
    _uint m_iNumRut = { 0 };//����
    _uint m_iNumPelt = { 0 };//����

    _uint m_iState = HARVEST_SELECT;    
public:
    static CHarvest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual void Free() override;
};

END