#pragma once
#include "Client_Defines.h"
#include "Base.h"

#include "UIObject.h"
#include "UITEXT.h"
#include "UIBack.h"
#include "UIColor.h"
#include "UIToggleID.h"
#include "UISortToggle.h"
#include "UIButton.h"
#include "UIItemIcon.h"
#include "UIItemToggle.h"
#include "UIStateCloth.h"
#include "UIClothToggle.h"
#include "UILoadingBar.h"

BEGIN(Engine)
class CGameInstance;
class CItem;
class CGameObject;
class CItemData;
END 

BEGIN(Client)
class CUImanager :
    public CBase
{
public: 
    enum LayerIndex {
        Layer_RayColl, Layer_KEY, Layer_Base, Layer_Type, Layer_Text, Layer_InvenBack, Layer_InvenCase,
        Layer_Toggle, Layer_SortBase, Layer_SortIcon ,Layer_NameText, Layer_InvenText, 
        Layer_InvenButton,  Layer_Item, Layer_StateBody,
        Layer_StateShoes, Layer_StatePant, Layer_StateSleeve,Layer_StateHat
        , Layer_StateInfoback, Layer_StateInfo,
        Layer_StateInfoText, Layer_Stateback, Layer_ClothCase, Layer_ClothText, Layer_ClothToggle,
        Layer_LoadingBar, Layer_END
    };
public:
    enum KEYID{ KEY_LEFTCLICK, KEY_RIGHTCLICK, KEY_END };
    enum BASEID{ BASE_BAG, BASE_WEIGHT, BASE_DURABILITY, BASE_END};
    enum TEXTID { TEXT_NAME, TEXT_INFO, TEXT_WEIGHT, TEXT_DURABILITY,
        TEXT_LCLICK, TEXT_RCLICK, TEXT_BAGWEIGHT, TEXT_TOTALWEIGHT, TEXT_END};
    //	enum ITEMTYPE {   ITEM_STUFF,  ITEM_MEDKITS,   ITEM_KINDLING, ITEM_CLOTH,    ITEM_FOOD, ITEM_EQUIPMENT,      ITEM_END};

    enum TypeID {TYPE_ONE , TYPE_TWO , TYPE_END};
    
    //FOR INVENTORY
public:
    enum BACK { BACK_FOCUS, BACK_CLOUD, BACK_NOTEXTURE, BACK_END };
    enum CASE {CASE_ONE, CASE_END};
    enum TOGGLE { TOGGLE_OFF , TOGGLE_ON , TOGGLE_END};
    enum SELECT {SELECT_BASE ,SELECT_END};
    enum SCENE {SCENE_STATE, SCENE_EQUIP, SCENE_INVEN, SCENE_BLUEPRINT, SCENE_END};
    enum SORT{SORT_ALL, SORT_KINDLING, SORT_MEDIKIT, SORT_CLOTH, SORT_FOOD, SORT_EQUIP, SORT_STUFF , SORT_END};
    enum SCENENAME { SCENETEXT_NAME , SCENETEXT_END};
    enum IVENTEXTID {
         INVENTEXT_TOTALWEIGHT, INVENTEXT_BAGWEIGHT, INVENTEXT_BACK,  INVENTEXT_SORTMODE,  INVENTEXT_END
    };
    enum INVENBUTTON{ INVENBUTTON_BACK,  INVENBUTTON_END};
public:
    enum CLOTH {CLOTH_SHOE ,CLOTH_PANTS, CLOTH_SWEATER, CLOTH_HET, CLOTH_END};
private:
    CUImanager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CUImanager() = default;
public:
     HRESULT Initialize() ;
     HRESULT Item_SetInfo(class CGameObject* pPickObject, _float fTotalWeight);
     void Render_UI(_uint LayerIndex);
     void Render_TypeUI(_uint LayerIndex);
     void Render_Cloth();

     void Scene_Change(_uint iUIIndex);
     void Sort_IconChange(_uint iSortIndex);
     
     HRESULT Ready_RayColl();
     HRESULT Ready_PickUpSelecter();
     HRESULT Ready_Inventory();
     HRESULT Ready_Itemcase();
     HRESULT Ready_State();
     HRESULT Ready_Cloth();
     HRESULT Ready_LoadingBar();

     void Set_SortMode(_uint SortIndex);
     _uint Get_SortMode() { return m_SortMode; }
public:
    _uint Pick_UISceneToggle(_uint UIID);
    _uint Pick_UISortToggle(_uint UIID);
    void Pick_InvenItem(_uint UIID);
    void Pick_ClothItem(_uint UIID);
    _bool Pick_InvenInfoButton();
    _bool Pick_InvenClothButton();
public:
    //���� ��ȯ (ƽ����
    void Set_Weight(_float fTotalWeight);
    HRESULT Cloth_Update(class CItemData* pItemData);

public :
    HRESULT Add_InvenIcon(_uint Index,  class CItem* pItem);
    HRESULT Add_InvenIcon(_uint Index,  class CItemData* pItemData);
    HRESULT Add_ClothIcon(class CItemData* pItemData, void* ClothDesc);
    HRESULT Sort_Reset();
    HRESULT Cloth_Reset();
    HRESULT Sort_InvenIcon(_uint Index,  class CItemData* pItemData);

public://raycoll ���� �ֱ�
     HRESULT RayColl_SetInfo(const wstring ObjName);
private:
    class CGameInstance*             m_pGameInstance = { nullptr };
    class ID3D11Device*             m_pDevice = { nullptr };
    class ID3D11DeviceContext* m_pContext = { nullptr };
    //ui�� ���� ǥ�ÿ� ���� �͵�(������Ÿ��), �⺻ ����ǰ(����,����,������)
    //�����¿� Ŭ��������
    vector<wstring> m_UISelector;
    vector<class CUIObject*> m_TypeUI;

    vector<CGameObject*> m_ItemToggle;//������ ĭ ������(�׸��� �� �޾ƿ�
    vector<CGameObject*> m_ClothToggle;//�� ĭ ������(�׸��� �� �޾ƿ�
    _int m_PickItemIndex = { -1 };//���� ����ε��� ��ȣ
    _int m_PickClothIndex = { -1 };//���� ����ε��� ��ȣ
private:
    _int m_UIID = { 0 };
    SORT m_SortMode = { SORT_END };
private:
    //������ ĭ �ʱ���ġ�� ���� ����x,y
    _float2 m_ItemcasePos = { 0.f, 0.f };
    _float2 m_ItemcaseInterval = { 0.f, 0.f };
    //���μ��� ���� ũ�� ����
    _float m_fX, m_fY;
public:
    static CUImanager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;
};

END