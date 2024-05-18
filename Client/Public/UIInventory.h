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
class CUIInventory :
    public CBase
{
public:
    typedef struct {
        void* PlayerInventory;
        void* UImanager;
    }INVENTORY_DESC;
public:
    enum SCENE { SCENE_STATE, SCENE_EQUIP , SCENE_BAG , SCENE_BLUEPRINT , SCENE_END };
    enum SORT { SORT_ALL, SORT_KINDLING, SORT_MEDIKIT, SORT_CLOTH, SORT_FOOD, SORT_EQUIP, SORT_STUFF, SORT_END };
    enum INVENBUTTON { INVENBUTTON_BACK,  INVENBUTTON_END };

private:
    CUIInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CUIInventory() = default;
public:
    HRESULT Initialize(void* pArg);
    HRESULT Tick(_float fTimeDelta);
    HRESULT Late_Tick(_float fTimeDelta);
    void Render_UI(_uint LayerIndex);
    void Render_TypeUI(_uint LayerIndex);

public:
    void Set_CurrentScene(_uint iSceneIndex);
    void Set_SortMode(_uint iSortIndex);
    void Refresh();
    void Pick_UIToggle();
    void Cloth_Update();

private:
    class CGameInstance* m_pGameInstance = { nullptr };
    class ID3D11Device* m_pDevice = { nullptr };
    class ID3D11DeviceContext* m_pContext = { nullptr };

    //레이어 이름을 가지고 있음
    class CInventory* m_pPlayerInventory = { nullptr };
    class CUImanager* m_pUImanager = { nullptr };
private:
    SCENE m_CurrentScene = { SCENE_END };

    //가로세로 절반 크기 저장
    _float m_fX, m_fY;
public:
    static CUIInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg);
    virtual void Free() override;
};
END
